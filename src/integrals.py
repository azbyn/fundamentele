def exp(x, iterations=20):
    factorial = 1
    pow = 1
    res = 1.0
    for i in range(1, iterations):
        factorial *= i
        pow *= x
        res += pow/factorial
    return res

def cos(x, iterations=20):
    factorial = 1
    pow = 1
    res = 1.0
    for i in range(1, iterations, 2):
        factorial *= i * (1+i)
        pow *= - x * x
        res += pow/factorial
    return res


def integrate(f, a, b, eps=1e-4):
    delta = b - a
    if delta >= eps: 
        c = (a+b)/2
        return integrate(f, a, c, eps) + integrate(f, c, b, eps)
    return delta * (f(a) + f(b)) / 2
print("$ python3 integrals.py")
from math import pi
print("e^x:", integrate(exp, 0, 1))# should be e - 1
print("x^2:", integrate(lambda x: x*x, -1, 1))# should be 2/3
print("cos:", integrate(cos, -pi/2, pi/2))# should be 2
