best = []
from sys import argv
iter_ = 0
def find_seq(v, i, res= []):
    global best, iter_
    #iter_ += 1
    n = len(v)
    delta = len(best) - len(res)
    if len(res) > len(best):
        best = res
    for j in range(i+1, n):
        if v[i] <= v[j]:
            if n - j < delta+1: continue
            for k in range(j+1, n):
                if v[j] >= v[k]:
                    if n - k < delta+2: continue
                    find_seq(v, k, res +[v[j],v[k]])


print("$ python3 src.py")
print("Please insert the set:", argv[1])
strs = argv[1].split(' ')
v = [int(str) for str in strs if str != ""]
for i in range(len(v)):
    if len(v) - i < len(best): continue
    find_seq(v, i, [v[i]])

if len(best) >= 3: print("Result:", best)
else: print("No such sequence found")
