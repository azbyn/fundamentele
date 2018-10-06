#include <iostream>
#include <algorithm>

std::string toBase2(unsigned n) {
    if (n == 0) return "0";
    std::string res = "";
    while (n) {
        res += ('0' + (n % 2));
        n /= 2;
    }
    std::reverse(res.begin(), res.end());
    return res;
}


std::string toBase16(unsigned n) {
    if (n == 0) return "0";
    std::string res = "";
    while (n) {
        int mod = n % 16;
        res += (mod < 10) ? ('0' + mod) : ('A' - 10 + mod);
        n /= 16;
    }
    std::reverse(res.begin(), res.end());
    return res;
}

int main() {
    int n;
    std::cout << "Please enter a number: ";
    if (!(std::cin >> n)) {
        std::cerr << "That was not a number!\n";
        return -1;
    }
    /* 
    //WIP
    std::cout << n << "is ";

    int pow2 = 1 << (sizeof(int) * 8 - 1);
    while (! (n & )  )
    */

    std::cout << n << " is '" << toBase2(n) << "' in base 2 and '"
              << toBase16(n) << "' in base 16\n";
    return 0;
}
