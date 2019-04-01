from math import pow, sqrt
import sys

t = (pow(19-3*sqrt(33), 1/3)+pow(19+3*sqrt(33), 1/3)+1)/3
b = pow(586+ 102*sqrt(33), 1/3)
left = (3*b) / (b*b - 2 * b + 4)
def f(n):
    # this gives the correct answer from 1 to 53
    if n < 54: return round(left * pow(t, n+1))
    t1 = f(53)
    t2 = f(52)
    t3 = f(51)
    n -= 53
    while n > 0:
        n -= 1
        r = t1 + t2 +t3
        t3 = t2
        t2 = t1
        t1 = r
    return t1

print("$ python3 src.py")
print("Please insert a number (> 0):", sys.argv[1])
n = int(sys.argv[1])
print(f(n))
