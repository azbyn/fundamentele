import sys
print("$ python3 counting_sort.py")
print("Please insert the array:", sys.argv[1])
x = [int(num) for num in (sys.argv[1].split(' '))]

n = len(x)
m = max(x) + 1

f = [0 for i in range(m)]
output = [0 for i in range(n)]

for i in x: f[i] += 1
print("f:", f)

for i in range(1, m):
    f[i] = f[i-1] + f[i]


print("fc:", f)
for i in range(n):
    f[x[i]] -= 1
    output[f[x[i]]] = x[i]

print("output:", output)
