#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#include <linux/kd.h>
#include <linux/keyboard.h>
#include <sys/ioctl.h>
#include <linux/input-event-codes.h>

//#include <locale.h>
#include <errno.h>

#include <stdexcept>


#define logfln(x, ...) fprintf(stderr, "> " x "\n", __VA_ARGS__)
#define logf(x, ...) fprintf(stderr, x, __VA_ARGS__)


/*extern */int getfd();

int fd;
int oldkbmode;
struct termios old;
/*
* version 0.81 of showkey would restore kbmode unconditially to XLATE,
* thus making the console unusable when it was called under X.
*/
static int get_mode() {
    int mode;
    const char *m;
    if (ioctl(fd, KDGKBMODE, &mode)) {
        perror("KDGKBMODE");
        exit(1);
    }
    switch(mode) {
    case K_RAW:
        m = "RAW"; break;
    case K_XLATE:
        m = "XLATE"; break;
    case K_MEDIUMRAW:
        m = "MEDIUMRAW"; break;
    case K_UNICODE:
        m = "UNICODE"; break;
    default:
        m = "?UNKNOWN?"; break;
    }
    printf("kb mode was %s\n", m);
    if (mode != K_XLATE) {
        printf("[ if you are trying this under X, it might not work\n"
            "since the X server is also reading /dev/console ]\n");
    }
    printf("\n");
    return mode;
}
static void clean_up() {
    if (ioctl(fd, KDSKBMODE, oldkbmode)) {
        perror("KDSKBMODE");
        exit(1);
    }
    if (tcsetattr(fd, 0, &old) == -1)
        perror("tcsetattr");
    close(fd);
}
static void die(int x) {
    printf("caught signal %d, cleaning up...\n", x);
    clean_up();
    exit(1);
}
void init_signal_handler() {
    signal(SIGALRM, die);
    signal(SIGHUP, die);
    signal(SIGINT, die);
    signal(SIGQUIT, die);
    signal(SIGILL, die);
    signal(SIGTRAP, die);
    signal(SIGABRT, die);
    signal(SIGIOT, die);
    signal(SIGFPE, die);
    signal(SIGKILL, die);
    signal(SIGUSR1, die);
    signal(SIGSEGV, die);
    signal(SIGUSR2, die);
    signal(SIGPIPE, die);
    signal(SIGTERM, die);
#ifdef SIGSTKFLT
    signal(SIGSTKFLT, die);
#endif
    signal(SIGCHLD, die);
    signal(SIGCONT, die);
    signal(SIGSTOP, die);
    signal(SIGTSTP, die);
    signal(SIGTTIN, die);
    signal(SIGTTOU, die);
}

int main(/*int argc, char *argv[]*/) {
    struct termios new_;
    unsigned char buf[16];
    bool show_keycodes = true;
    //Likely useless:
    //setlocale(LC_ALL, "");

    fd = getfd();
    init_signal_handler();

    oldkbmode = get_mode();
    if (tcgetattr(fd, &old) == -1)
        perror("tcgetattr");
    if (tcgetattr(fd, &new_) == -1)
        perror("tcgetattr");
    new_.c_lflag &= ~(ICANON | ECHO | ISIG);
    new_.c_iflag = 0;
    new_.c_cc[VMIN] = sizeof(buf);
    new_.c_cc[VTIME] = 1; /* 0.1 sec intercharacter timeout */
    if (tcsetattr(fd, TCSAFLUSH, &new_) == -1)
        perror("tcsetattr");
    if (ioctl(fd, KDSKBMODE, show_keycodes ? K_MEDIUMRAW : K_RAW)) {
        perror("KDSKBMODE");
        exit(1);
    }

    constexpr int modKeysCount = 3;
    constexpr uint8_t modKeys[modKeysCount][2] = {
        {KEY_LEFTSHIFT, KEY_RIGHTSHIFT},
        {KEY_LEFTCTRL, KEY_RIGHTCTRL},
        {KEY_LEFTMETA, KEY_RIGHTMETA}
    };

    bool modPresses[modKeysCount][2] = {};
    auto hasModPress = [&modPresses] (int i) { return modPresses[i][0] || modPresses[i][1]; };
    const int modkey = KEY_CAPSLOCK; // 125;
    bool pressing_mod = false;

    for (;;) {
        int n = read(fd, buf, sizeof(buf));
        for (int i = 0; i < n; i++) {
            char keycode = buf[i] & 0x7f;
            bool released = buf[i] & 0x80;
            for (int j = 0; j < modKeysCount; ++j) {
                for (int k = 0; k < 2; ++k){
                    if (keycode == modKeys[j][k]) {
                        modPresses[j][k] = !released;
                        goto endModkeySearch;
                    }
                }
            }

            printf("key ");
            if (hasModPress(0)) printf("SHIFT-");
            if (hasModPress(1)) printf("CTRL-");
            if (hasModPress(2)) printf("META-");

            printf("%3d\n", keycode);


        endModkeySearch:

            if (keycode == modkey) {
                if (pressing_mod != !released)
                    logf("\n<%smod>\n", released ? "\\" : "");
                pressing_mod = !released;
            }
            else if (keycode == 0) break;
            else {//if (pressing_mod && !released) {
            }
        }
    }
    clean_up();

    return 0;
}



/*
* getfd.c
*
* Get an fd for use with kbd/console ioctls.
* We try several things because opening /dev/console will fail
* if someone else used X (which does a chown on /dev/console).
*/
static int is_a_console(int fd) {
    char arg;
    arg = 0;
    return (ioctl(fd, KDGKBTYPE, &arg) == 0 &&
            ((arg == KB_101) || (arg == KB_84)));
}
static int open_a_console(const char *fnam) {
    int fd;
    /*
    * For ioctl purposes we only need some fd and permissions
    * do not matter. But setfont:activatemap() does a write.
    */
    fd = open(fnam, O_RDWR);
    if (fd < 0 && errno == EACCES)
        fd = open(fnam, O_WRONLY);
    if (fd < 0 && errno == EACCES)
        fd = open(fnam, O_RDONLY);
    if (fd < 0)
        return -1;
    if (!is_a_console(fd)) {
        close(fd);
        return -1;
    }
    return fd;
}
#define GETFD_IMPL(_path)               \
    fd = open_a_console(_path);         \
    if (fd >= 0) {                      \
        logfln("Opened '%s'", _path);    \
        return fd;                      \
    }

int getfd() {
    int fd;
    GETFD_IMPL("/dev/tty");
    GETFD_IMPL("/dev/tty0");
    GETFD_IMPL("/dev/vc/0");
    GETFD_IMPL("/dev/console");

    for (fd = 0; fd < 3; fd++) {
        if (is_a_console(fd)) {
            logfln("Opened '%d'", fd);
            return fd;
        }
    }
    throw std::runtime_error("Couldnt get a file descriptor referring to the console\n");
}
#undef GETFD_IMPL
