import sys
print("Please insert the sequence: "+sys.argv[1])

strs = sys.argv[1].split(' ')
x = [int(num) for num in strs]
n = len(x)
f = [0 for i in range(n)]

f[0] = x[0]
for i in range(1, n):
    f[i] = f[i-1] + x[i]

print(f)
