from sys import argv
print("Please insert the set:", argv[1])
A = [int(num) for num in (argv[1].split(' ')) if num != ""]
print("Please insert s:", argv[2])
s = int(argv[2])

size = len(A)
x = [-1 for i in range(size+1)]
k = 0
index = 0
while k >= 0:
    if x[k] < size-1:
        x[k] += 1
        sum = A[x[k]]
        def valid():
            global sum
            for i in range(0, k):
                if x[k] == x[i]:
                    return False
                sum += A[x[i]]
            return True
        if not valid() or sum > s: continue

        if sum == s:
            print("{", end="")
            for i in x[:k]: print(A[i], end=", ")
            print(A[x[k]], end="}, ")
            #if index % 6 == 5: print()
            #index += 1
            #break
        if k != size:
            x[k+1] = x[k]-1
            k += 1
    else:
        k -= 1
print()
