#include <iostream>
#include <algorithm>

int main() {
    int n;
    std::cout << "Please enter a number: ";
    if (!(std::cin >> n)) {
        std::cerr << "That was not a number!\n";
        return -1;
    }
    std::cout << n << " is '";
    if (n == 0) {
        std::cout << "0' in base 2 and '0' in base 16 \n";
        return 0;
    }
    // base 2
    unsigned pow2 = 1 << (sizeof(unsigned) * 8 - 1);
    while (!(n & pow2)) pow2 >>= 1;
    while (pow2) {
        std::cout << (char)('0' + (!!(n & pow2)));
        pow2 >>= 1;
    }
    std::cout << "' in base 2 and '";
    // base 16
    std::string res = "";
    while (n) {
        int mod = n % 16;
        res += (mod < 10) ? ('0' + mod) : ('A' - 10 + mod);
        n /= 16;
    }
    std::reverse(res.begin(), res.end());

    std::cout << res << "' in base 16\n";

    return 0;
}
