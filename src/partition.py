def partition(v):
    size = len(v)
    i = -1
    j = size
    while True:
        i += 1
        while i < size and v[i] <= 0: i += 1
        j -= 1 
        while j >= 0 and v[j] >= 0: j -= 1
        
        if i < j:
            v[i], v[j] = v[j], v[i]
        else:
            return


import sys
print("$ python3 src.py")
print("Please insert the vector: "+sys.argv[1])
strs = sys.argv[1].split(' ')
v = [int(str) for str in strs if str != ""]

partition(v)
print(v)
