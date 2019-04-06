def permutations(v):
    def impl(v, n):
        if n == 1: 
            yield v
            return
        for i in range(n):
            for p in impl(v, n-1):
                yield p
            if n % 2 == 1:
                v[0], v[n-1] = v[n-1], v[0]
            else:
                v[i], v[n-1] = v[n-1], v[i]
    return impl(v, len(v))

from sys import argv
print("$ python3 perm.py")
print("Please insert n:", argv[1])
n = int(argv[1])
v = [i for i in range(1, n+1)]

i = 0
for p in permutations(v):
    print("(", end="")
    for v in p[:-1]: print(v, end=" ")
    print(p[-1], end="")
    print(")", end=", ")
    if i % 6 == 5: print()
    i += 1
