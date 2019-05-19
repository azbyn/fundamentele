    #v = [ 9, 1, 2, 3, 2, 5, 4, 6, 7, 2, 2
from itertools import product, permutations
worst_l = 0
worst_v = []
worst_b = []
w = permutations([i for i in range(9)])
w = [[0 for i in range(12+3)]]
w = [[(i%4)//2 for i in range(30)]]
w =product([i for i in range(2)], repeat=12)
avrg = 0
itt = 0
for v in w:
    #v = [ i%2 for i in range(21)]+[3+ i%2 for i in range(80)]
    #expected 1 <2> 2<5>4<6>2

    best = [1]
    iter_ = 0
    def find_seq(v, i, res= []):
        global best, iter_
        iter_ += 1
        n = len(v)
        if len(res) > len(best):
            best = res
        delta = len(best) - len(res)
        for j in range(i+1, n):
            if v[i] <= v[j]:
                if n - j < delta+1: continue
                for k in range(j+1, n):
                    if v[j] >= v[k]:
                        if n - k < delta+2: continue
                        find_seq(v, k, res +[v[j],v[k]])


    #print("Please insert the set: ", end="")
    #strs = input().split(' ')
    #s = [int(str) for str in strs if str != ""]
    for i in range(len(v)):
        if len(v) - i <= len(best): continue
        #print("JAVLA")
        find_seq(v, i, [v[i]])

    #print(best)
    #print(iter_,"for", len(v))
    avrg += iter_
    itt += 1
    if iter_ > worst_l:
        worst_l = iter_
        worst_v = v
        worst_b = best

print(worst_l,":", worst_v, ";", worst_b)
print(avrg, ":", avrg/ itt )
