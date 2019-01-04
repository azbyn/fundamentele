#include <iostream>
#include <stdexcept>
#include <fstream>
#include <array>
#include <ctime>
#include <complex>
#include <thread>

#include <stdio.h>
#include <stdint.h>


struct Complex {
    double re;
    double im;
    constexpr Complex(double re, double im) : re(re), im(im) {
    }
    constexpr Complex() : re(0), im(0) {
    }
    constexpr Complex(std::complex<double> s) : re(std::real(s)), im(std::imag(s)) {}
    constexpr double mod2() const {
        return re*re + im*im;
    }
    constexpr Complex operator+(const Complex& rhs) const {
        return Complex(rhs.re+re, rhs.im+im);
    }
    constexpr Complex operator*(const Complex& rhs) const {
        return Complex(re * rhs.re - im * rhs.im,
                       re * rhs.im + im * rhs.re);
    }

    //constexpr Complex 
    constexpr std::complex<double> toStd() const {
        return std::complex<double>(re, im);
    }
    friend std::ostream& operator <<(std::ostream& s, const Complex& rhs) {
        return s << "(" << rhs.re << " + " << rhs.im << "i)";
    }
};
constexpr int maxIter = 10;//50;
constexpr double mandelbrotMax = 2;// 20000000;
double mandelbrotColor(std::complex<double> c) {
    using namespace std;
    int i;
    auto z = complex<double>(0, 0); //c;//Complex(0, 0);
    for (i = 1; i <= maxIter; ++i) {
        //z = z*z+c;
        z = z*z*z*complex<double>(1,0)+z*z*complex<double>(.2,1.7) + tan(sin(complex<double>(0.9, 1.7)* c*c*c));
//z = z*z *z + c;
        //z = z * z + Complex(-0.74543,0.11301); //0.279, 0);
        //std::cout << z << "\n";
        if (norm(z) > mandelbrotMax*mandelbrotMax) break;
    }
    double iter = i;
    /*
    if (i < maxIter) {
        // sqrt of inner term removed using log simplification rules.
        auto log_zn = log(norm(z)) / 2;
        auto nu = log( log_zn / log(2) ) / log(2);
        // Rearranging the potential function.
        // Dividing log_zn by log(2) instead of log(N = 1<<8)
        // because we want the entire palette to range from the
        // center to radius 2, NOT our bailout radius.
        iter += 1 - nu;
    }
    */
    return iter / maxIter;
}

constexpr int pixelsPerUnit = 128;// 1024;
constexpr double xMin = -3;//-2.1;
constexpr double xMax = -xMin;//1;//2;
constexpr double yMin = -6;//1.3;
constexpr double yMax = -yMin;
constexpr int threadCount = 50;

constexpr size_t width = size_t((xMax - xMin) * pixelsPerUnit);
constexpr size_t height = size_t((yMax - yMin) * pixelsPerUnit);

constexpr size_t getBounds(const int i) {
    if (i == threadCount) return height;
    else if (i == 0) return 0;
    else return (height * i)/ threadCount;
}

constexpr double lerp(double v0, double v1, double t) {
    return v0 + t * (v1 - v0);
}

uint8_t data[width*height];

void doWork(int i) {
    //std::cout << "WORK " << i<< " FROM "<< getBounds(i) << " TO " << getBounds(i+1) <<"\n";
    for (size_t y = getBounds(i), end = getBounds(i+1); y < end; ++y) {
        for (size_t x = 0; x < width; ++x) {
            std::complex<double> pos(lerp(xMin, xMax, x / double(width)),
                                     lerp(yMin, yMax, y / double(height)));
            uint8_t col = mandelbrotColor(pos) * 255;
            data[y * width + x] = col;
        }
    }
}

int main() {
    std::thread threads[threadCount];
    for (int i = 0; i < threadCount; ++i)
        threads[i] = std::thread(doWork, i);

    for (int i = 0; i < threadCount; ++i)
        threads[i].join();

    /*
    std::ofstream file("file.pgm", std::ios_base::binary & std::ios_base::out);

    file << "P5\n" << width << " " << height << "\n" << "255\n";
    for (auto a : data)
        file << a;
    */
    //about 30 times faster than the ofstream equivalent
    FILE* fp = fopen("file.pgm", "wb");
    /* write header to the file */
    fprintf(fp, "P5\n %lu\n %lu\n 255\n", width, height);
    /* write image data bytes to the file */
    fwrite(data, sizeof(data), 1, fp);
    fclose(fp);

    return 0;
//std::cout << "NIET\n";
//std::cout << mandelbrotColor(Complex(3.0001/4,0));
}
