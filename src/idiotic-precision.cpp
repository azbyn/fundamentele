#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>

using byte = uint8_t;


constexpr size_t byteCnt(uint64_t val) {
    if (val < 1lu << (8*1)) return 1;
    if (val < 1lu << (8*2)) return 2;
    if (val < 1lu << (8*3)) return 3;
    if (val < 1lu << (8*4)) return 4;
    if (val < 1lu << (8*5)) return 5;
    if (val < 1lu << (8*6)) return 6;
    if (val < 1lu << (8*7)) return 7;
    return 8;
}
constexpr char hexChar(int i) {
    if (i < 10) return '0' + i;
    return 'A' - 10 + i;
}
class IPInt {
public:
    std::vector<byte> data;
    enum Sign {
        SGN_Negative = -1,
        SGN_Zero = 0,
        SGN_Positive = 1,
    };
    Sign sign;

    IPInt(std::vector<byte> data, Sign sign) : data(data), sign(sign) {
    }

public:
    IPInt() : data{}, sign{SGN_Zero} {
    }
    IPInt(long val) {
        if (val == 0) {
            sign = SGN_Zero;
            return;
        }
        if (val < 0) {
            sign = SGN_Negative;
            val = -val;
        } else {
            sign = SGN_Positive;
        }
        new (&data) std::vector<byte>(byteCnt(val));
        for (auto& b : data) {
            b = (byte) val;
            val >>= 8;
        }
    }

    constexpr bool isZero() const {
        return sign == SGN_Zero;
    }
    std::string toHex() const {
        std::string res;
        switch (sign) {
        case SGN_Zero: return "0";
        case SGN_Negative: res += "-"; break;
        case SGN_Positive: res += "+"; break;
        }

        for (auto i = data.rbegin(), end = data.rend(); i != end; ++i) {
            res += hexChar(*i / 16);
            res += hexChar(*i % 16);
        }
        return res;
    }
    std::string toDec() const {
        std::string res;
        switch (sign) {
        case SGN_Zero: return "0";
        case SGN_Negative: res += "-"; break;
        case SGN_Positive: break;
        }
        IPInt a = *this;
        //we can have at most 3 digits per byte
        std::vector<char> c(a.data.size() * 3);
        int i = 0;
        while (!a.isZero()) {
            c[i++] = '0' + a.divmod64_unsigned_unchecked(10);
        }
        do {
            std::cout << c[--i];
        } while (i != 0);

        return res;
    }
    std::string toString() const {
        return toDec();
    }
    IPInt operator+() const {
        return *this;
    }
    IPInt operator-() const {
        switch (sign) {
        case SGN_Zero: return *this;
        case SGN_Negative: return IPInt(data, SGN_Positive);
        case SGN_Positive: return IPInt(data, SGN_Negative);
        }
    }
    IPInt operator+(const IPInt& rhs) const {
        IPInt res = *this;
        return res += rhs;
    }
    IPInt& operator+=(const IPInt& rhs) {
        return addImpl<false>(rhs);
    }
    IPInt operator-(const IPInt& rhs) const {
        IPInt res = *this;
        return res -= rhs;
    }
    IPInt& operator-=(const IPInt& rhs) {
        return addImpl<true>(rhs);
    }
    int64_t toInt64() const {
        if (sign == SGN_Zero) return 0;
        if (data.size() > sizeof(int64_t)) {
            throw std::runtime_error("Number too big for int64");
        }
        int64_t res = 0;
        int i = 0;
        for (auto& b : data) {
            res |= (b << i);
            i += 8;
        }
        if (sign == SGN_Negative) return -res;
        return res;
    }
    IPInt operator <<(uint64_t rhs) const {
        IPInt res = *this;
        return res <<= rhs;
    }
    IPInt operator <<(const IPInt& rhs) const {
        return *this << rhs.toInt64();
    }
    IPInt& operator <<=(const IPInt& rhs) {
        return *this <<= rhs.toInt64();
    }
    IPInt& operator <<=(uint64_t rhs) {
        data.resize(data.size() + rhs / 8 + 1);

        uint64_t smallShift = rhs % 8;
        uint16_t carry = 0;
        for (auto& b : data) {
            carry = (b << smallShift) | carry;
            b = carry;
            carry >>= 8;
        }
        //big Shift
        auto beg = data.begin();
        auto mid = beg + (rhs / 8);
        std::copy(beg, data.end(), mid);
        std::fill(beg, mid, 0);

        clearZeroes();
        return *this;
    }
    IPInt operator >>(uint64_t rhs) const {
        IPInt res = *this;
        return res >>= rhs;
    }
    IPInt operator >>(const IPInt& rhs) const {
        return *this >> rhs.toInt64();
    }
    IPInt& operator >>=(const IPInt& rhs) {
        return *this >>= rhs.toInt64();
    }

    IPInt& operator >>=(uint64_t /*rhs*/) {
        throw std::logic_error("Maybe I should implemnt left shift");
    }

    IPInt operator |(const IPInt& rhs) const {
        IPInt res = *this;
        return res |= rhs;
    }
    IPInt& operator |=(const IPInt& rhs) {
        if (rhs.isZero()) return *this;
        if (isZero()) return *this = rhs;
        if (sign == SGN_Negative)
            throw std::logic_error("Can't bitwise or negative numbers");

        data.resize(std::max(data.size(), rhs.data.size()));

        //left(right) iterator
        auto li = data.begin();
        auto ri = rhs.data.begin();
        // right is the smallest (avoid going out of range)
        auto rend = rhs.data.end();
        for (; li != rend; ++li, ++ri) {
            *li |= *ri;
        }
        //clearZeroes();
        return *this;
    }
    IPInt operator &(const IPInt& rhs) const {
        IPInt res = *this;
        return res &= rhs;
    }
    IPInt& operator &=(const IPInt& rhs) {
        if (rhs.isZero()) return *this;
        if (isZero()) return *this = rhs;
        if (sign == SGN_Negative)
            throw std::logic_error("Can't bitwise or negative numbers");

        data.resize(std::min(data.size(), rhs.data.size()));

        //left(right) iterator
        auto li = data.begin();
        auto ri = rhs.data.begin();
        // left is the smallest (avoid going out of range)
        auto lend = data.end();
        for (; li != lend; ++li, ++ri) {
            *li &= *ri;
        }
        clearZeroes();
        return *this;
    }

    bool operator ==(const IPInt& rhs) const {
        if (sign != rhs.sign) return false;
        if (sign == SGN_Zero) return true;
        if (data.size() != rhs.data.size()) return false;
        auto it = data.rbegin();
        auto rit = rhs.data.rbegin();
        auto end = data.rend();
        for (; it != end; ++it, ++rit) {
            if (*it != *rit) return false;
        }
        return true;
    }
    bool operator !=(const IPInt& rhs) const { return !(*this == rhs); }
    bool operator <=(const IPInt& rhs) const { return cmp(rhs) <= 0; }
    bool operator >=(const IPInt& rhs) const { return cmp(rhs) >= 0; }
    bool operator <(const IPInt& rhs) const { return cmp(rhs) < 0; }
    bool operator >(const IPInt& rhs) const { return cmp(rhs) > 0; }

    //might return -2 or 2
    int cmp(const IPInt& rhs) const {
        if (sign != rhs.sign) return sign - rhs.sign;
        //?
        if (sign == SGN_Zero) return 0;
        if (data.size() > rhs.data.size()) return 1 * sign;
        if (data.size() < rhs.data.size()) return -1 * sign;
        auto it = data.rbegin();
        auto rit = rhs.data.rbegin();
        auto end = data.rend();
        for (;it != end; ++it, ++rit) {
            if (*it < *rit) return -1 * sign;
            if (*it > *rit) return 1 * sign;
        }
        return 0;
    }

    static constexpr byte getBit(byte b, int index) {
        return (b >> index) & 1;
    }

    // Return remainder
    // this gets modified to be the quotient
    uint64_t divmod64_unsigned(uint64_t div)  {
        if (div == 0) throw std::logic_error("Universe exploded (Division by 0)");
        if (isZero()) return 0;
        return divmod64_unsigned_unchecked(div);
    }
    uint64_t divmod64_unsigned_unchecked(uint64_t div)  {
        uint64_t rem = 0;
        auto it = data.rbegin();
        auto end = data.rend();
        for (; it != end; ++it) {
            for (int bitIndex = 7; bitIndex >= 0; --bitIndex) {
                rem <<= 1;
                //get bit
                rem |= (*it >> bitIndex) & 1;
                if (rem >= div) {
                    rem -= div;
                    //set bit
                    *it |= (1 << bitIndex);
                } else {
                    //unset bit
                    *it &= ~(1 << bitIndex);
                }
            }
        }
        clearZeroes();
        return rem;
    }

    /*
    void divmod_unsigned(const IPInt& rhs, IPInt& outQuot, IPInt& outRem) const {
        //this could be a lot faster
        if (rhs.isZero()) throw std::logic_error("Universe exploded (Division by 0)");
        new (&outQuot) IPInt();
        new (&outRem) IPInt();

        int bitIndex = 0;
        size_t byteIndex = 0;
        



        throw std::logic_error("This is too complicated for the moment. Please use divmod10 instead");
        }*/

private:
    void clearZeroes() {
        int i = data.size();
        for (; i > 0; --i) { if (data[i-1]) break; }
        data.resize(i);
        if (i == 0) sign = SGN_Zero;
    }

    int absCompareWithoutLast(const IPInt& rhs) const {
        if (data.size() - 1 == rhs.data.size()) {
            auto it = data.rbegin() + 1;
            auto rit = rhs.data.rbegin();
            auto end = data.rend();
            for (;it != end; ++it, ++rit) {
                if (*it < *rit) return -1;
                if (*it > *rit) return 1;
            }
            return 0;
        }
        return 1;
    }
    template<bool isNegated>
    IPInt& addImpl(const IPInt& rhs) {
        if (rhs.isZero()) return *this;
        if (isZero()) {
            if constexpr (isNegated) {
                *this = rhs;
                sign = (Sign) -sign;
                return *this;
            } else {
                return *this = rhs;
            }
        }
        Sign op;
        if constexpr (isNegated) {
            op = (Sign) -rhs.sign;
        } else {
            op = rhs.sign;
        }
        // just to be safe
        // redundant zeroes will be removed
        data.resize(std::max(data.size(), rhs.data.size()) + 1);

        //left(right) iterator
        auto li = data.begin();
        auto ri = rhs.data.begin();
        auto rend = rhs.data.end();
        //auto rend = rhs.data.end();
        int16_t carry = 0;
        if (sign == op) {
            for (; ri != rend; ++li, ++ri) {
                carry += *li + *ri;
                *li = (byte) carry;
                carry >>= 8;
            }
        } else {
            //try "Borrow?"
            auto cmp = [&]() {
                if (data.size() - 1 == rhs.data.size()) {
                    auto it = data.rbegin() + 1;
                    auto rit = rhs.data.rbegin();
                    auto end = data.rend();
                    for (;it != end; ++it, ++rit) {
                        if (*it < *rit) return -1;
                        if (*it == *rit) continue;
                        return 1;
                    }
                    return 0;
                }
                return 1;
            }();
            if (cmp == 0) {
                this->sign = SGN_Zero;
                this->data.resize(0);
                return *this;
            } else if (cmp > 0) {
                for (; ri != rend; ++li, ++ri) {
                    carry += *li - *ri;
                    *li = (byte) carry;
                    carry >>= 8;
                }
            } else {
                for (; ri != rend; ++li, ++ri) {
                    carry -= *li - *ri;
                    *li = (byte) carry;
                    carry >>= 8;
                }
                sign = (Sign) -sign;
            }
        }
        *li = carry;

        clearZeroes();
        return *this;
    }

public:
    friend std::ostream& operator<<(std::ostream& stream, const IPInt& int_) {
        //there are smarter ways to do this
        return stream << int_.toString();
    }
};



// 2^(2^0) + 2^(2^1) + ... 2^(2^n)
IPInt sum(size_t n) {
    // (n >= 13) // more than 1 kb
    // (n >= 20) // more than 1 mb
    // (n >= 34) // more than 1 gb
        
    if (n >= 35) {
        //we need about twice the size of int just to print it
        throw std::logic_error("You would need more than 4 gb of ram to calculate that");
    }
    IPInt res;
    for (size_t i = 0; i <= n; ++i) {
        res |= IPInt(1) << (1 << i);
    }
    return res;
}

int main(/*int argc, const char** argv*/) {
    int n;
    std::cout << "Please enter n: ";
    std::cin >> n;
    std::cout << "S(" << n << ") = " << sum(n);
#if 0
    if (argc <= 3) {
        std::cerr << "Please provide 2 numbers and an operator in this format:\n";
        std::cerr << "Num op Num\n";
        return -1;
    }
    long i, j;
    sscanf(argv[1], "%lx", &i);
    sscanf(argv[3], "%lx", &j);

    IPInt a(i);
    IPInt b(j);

    std::cout << "a: " << a << " (" << a.toInt64() << ")\n";
    std::cout << "b: " << b << " (" << b.toInt64() << ")\n";
    std::cout << "a" << argv[2] << "b = ";
    if (strcmp(argv[2], "+") == 0)       std::cout << a+b;
    else if (strcmp(argv[2], "-") == 0)  std::cout << a-b;
    else if (strcmp(argv[2], "<") == 0)  std::cout << (a<b);
    else if (strcmp(argv[2], ">") == 0)  std::cout << (a>b);
    else if (strcmp(argv[2], "<=") == 0) std::cout << (a<=b);
    else if (strcmp(argv[2], ">=") == 0) std::cout << (a>=b);
    else if (strcmp(argv[2], "==") == 0) std::cout << (a==b);
    else if (strcmp(argv[2], "!=") == 0) std::cout << (a!=b);
    else if (strcmp(argv[2], "<<") == 0) std::cout << (a<<b);
    else if (strcmp(argv[2], "|") == 0) std::cout << (a|b);
    else if (strcmp(argv[2], "&") == 0) std::cout << (a&b);
    else if (strcmp(argv[2], "/") == 0) std::cout << a.divmod64_unsigned(j)<< ", d: "<< a;
    //else if (strcmp(argv[2], ">>") == 0) std::cout << (a>>b);
    else
        throw std::runtime_error("Invalid operator");
    std::cout << "\n";
#endif
    return 0;
}
