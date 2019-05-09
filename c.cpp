#include <iostream>

void print(int n) {
    std::cout << n << " ";
}
// doar pt x >= 0
int round_(double x) {
    return int(x+0.5);
}

double pow_(double x, int n) {
    if (n == 1) return x;
    double r = pow_(x, n / 2);
    r = r * r;
    if (n % 2 == 1) r = r * x;
    return r;
}
int fib(int n) {
    if (n==0) return 0;
    double sqrt5 = 2.2360679775;

    return round_(1.0 / sqrt5 * 
                 (pow_((1+sqrt5)/2, n) - pow_(1-sqrt5/2, n)));
    // varianta mai eficienta
    // return round_(1.0 / sqrt5 * (pow_((1+sqrt5)/2, n)));
}
int main() {
    int n = 0;
    for (int n = 1; n < 10; ++n)
        print(fib(n));
    std::cout << "\n";
    return 0;
}
