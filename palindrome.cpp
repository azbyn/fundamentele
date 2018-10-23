#include <iostream>

int main() {
    int i;
    std::cout << "Please insert a number: ";
    std::cin >> i;

    int reverse = 0;
    int ogNum = i;
    while (i) {
        reverse *= 10;
        reverse += i % 10;
        i /= 10;
    }
    std::cout << "Reverse: " << reverse << "\n";
    std::cout << ogNum << (reverse == ogNum ? " is" : " is not") << " an palindrome\n";

    return 0;
}
