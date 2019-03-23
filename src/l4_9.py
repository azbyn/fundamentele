import sys
print("$ python3 src.py")
print("Please insert the sequence:", sys.argv[1])
strs = sys.argv[1].split(' ')
x = [int(num) for num in strs]
print("Please insert s:", sys.argv[2])

s = int(sys.argv[2])
def f(x):
    for i in range(0, len(x)):
        for j in range(i, len(x)):
            if x[i] + x[j] == s:
                print ("Found %d + %d = %d " % (x[i] , x[j], s))
                return True
    print("Not found")
    return False
f(x)

