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

//#include <locale.h>
#include <errno.h>

/*extern */int getfd(const char *fnam);

int fd;
int oldkbmode;
struct termios old;
/*
* version 0.81 of showkey would restore kbmode unconditially to XLATE,
* thus making the console unusable when it was called under X.
*/
static void
get_mode(void) {
    const char *m;
    if (ioctl(fd, KDGKBMODE, &oldkbmode)) {
        perror("KDGKBMODE");
        exit(1);
    }
    switch(oldkbmode) {
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
    if (oldkbmode != K_XLATE) {
        printf("[ if you are trying this under X, it might not work\n"
            "since the X server is also reading /dev/console ]\n");
    }
    printf("\n");
}
static void clean_up(void) {
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
static void watch_dog(int /*x*/) {
    clean_up();
    exit(0);
}

int main(/*int argc, char *argv[]*/) {
    int show_keycodes = 1;
    struct termios new_;
    unsigned char buf[16];
    int i, n;
    //Likely useless:
    //setlocale(LC_ALL, "");

    fd = getfd(NULL);
    /* the program terminates when there is no input for 10 secs */
    signal(SIGALRM, watch_dog);
    /*
        if we receive a signal, we want to exit nicely, in
        order not to leave the keyboard in an unusable mode
    */
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
    get_mode();
    if (tcgetattr(fd, &old) == -1)
        perror("tcgetattr");
    if (tcgetattr(fd, &new_) == -1)
        perror("tcgetattr");
    new_.c_lflag &= ~ (ICANON | ECHO | ISIG);
    new_.c_iflag = 0;
    new_.c_cc[VMIN] = sizeof(buf);
    new_.c_cc[VTIME] = 1;	/* 0.1 sec intercharacter timeout */
    if (tcsetattr(fd, TCSAFLUSH, &new_) == -1)
        perror("tcsetattr");
    if (ioctl(fd, KDSKBMODE, show_keycodes ? K_MEDIUMRAW : K_RAW)) {
        perror("KDSKBMODE");
        exit(1);
    }
    printf("press any key (program terminates 10s after last keypress)...\n");
    for (;;) {
        alarm(10);
        n = read(fd, buf, sizeof(buf));
        for (i = 0; i < n; i++) {
            if (!show_keycodes)
                printf("0x%02x ", buf[i]);
            else
                printf("keycode %3d %s\n",
                    buf[i] & 0x7f,
                    buf[i] & 0x80 ? "release"
                                    : "press");
        }
        if (!show_keycodes)
            printf("\n");
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
static int
is_a_console(int fd) {
    char arg;
    arg = 0;
    return (ioctl(fd, KDGKBTYPE, &arg) == 0
        && ((arg == KB_101) || (arg == KB_84)));
}
static int
open_a_console(const char *fnam) {
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
int getfd(const char *fnam) {
    int fd;
    if (fnam) {
        fd = open_a_console(fnam);
        if (fd >= 0)
            return fd;
        fprintf(stderr,
            "Couldnt open %s\n", fnam);
        exit(1);
    }
    fd = open_a_console("/dev/tty");
    if (fd >= 0)
        return fd;
    fd = open_a_console("/dev/tty0");
    if (fd >= 0)
        return fd;
    fd = open_a_console("/dev/vc/0");
    if (fd >= 0)
        return fd;
    fd = open_a_console("/dev/console");
    if (fd >= 0)
        return fd;
    for (fd = 0; fd < 3; fd++)
        if (is_a_console(fd))
            return fd;
    fprintf(stderr,
        "Couldnt get a file descriptor referring to the console\n");
    exit(1);		/* total failure */
}
