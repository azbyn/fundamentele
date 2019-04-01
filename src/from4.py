import sys
print("$ python3 src.py")

print("Please insert a number (> 0):", sys.argv[1])
n = int(sys.argv[1])

def from4(n):
    assert(n > 0)
    def impl(n, steps):
        steps.append(n)
        if n == 4: return steps
        if n % 10 == 4 or n % 10 == 0: return impl(n // 10, steps)
        return impl(n * 2, steps)
        
    steps = impl(n, [])
    for num in reversed(steps[1:]):
        print(num, "-> ", end="")
    print(n)

from4(n)
