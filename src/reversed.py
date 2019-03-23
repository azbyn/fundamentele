import sys
print("$ python3 reversed.py")

print("Please insert n:", sys.argv[1])
n = int(sys.argv[1])
res = 0
i = 0
while n != 0:
    res *= 10
    res += n % 10
    n //= 10
    i += 1

print("Result:", res)
