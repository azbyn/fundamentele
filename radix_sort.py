import sys
print("$ python3 radix_sort.py")
print("Please insert the array:", sys.argv[1])
strs = sys.argv[1].split(' ')
x = [int(num) for num in strs if num != ""]

n = len(x)

max_x = max(x)

f = [0 for i in range(10)]
output = [0 for i in range(n)]

pow10 = 1
while max_x > 0:
    def getDigit(num): return (num // pow10) % 10

    for i in range(10): f[i] = 0
    for i in x: f[getDigit(i)] += 1

    for i in range(1, 10): f[i] += f[i-1]

    for i in range(n - 1, -1, -1):
        index = getDigit(x[i])
        f[index] -= 1
        output[f[index]] = x[i]

    #output becomes new input
    for i in range(n): x[i] = output[i]

    pow10 *= 10
    max_x //= 10

print("output:", output)
