import sys
print("$ python3 shaker_sort.py")
print("Please insert the array:", sys.argv[1])
strs = sys.argv[1].split(' ')
v = [int(num) for num in strs]

def impl(start, end, step):
    sorted = True
    for i in range(start, end, step):
        if v[i] > v[i+1]:
            t = v[i]
            v[i] = v[i+1]
            v[i+1] = t
            sorted = False
    return sorted

begin = 0
end = len(v) - 1
while True:
    if impl(begin, end, 1): break
    if impl(end-1, begin-1, -1): break

    end -= 1
    begin += 1

print(v)
