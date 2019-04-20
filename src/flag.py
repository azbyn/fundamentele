#galben, verde, negru, alb, rosu, albastru, algbastru, -
print("$ python3 flags.py")
print("Long mode (y/N): n")
colors = ['g','v','n','a','r', 'b', '-']
index = 0

flag = [-1,0,0,0]
k = 0
maxK = 2

while k >= 0:
    maxVal = 6
    if k == 1: maxVal = 3
    elif k == 3: maxVal = 7
    if flag[k] < maxVal-1:
        flag[k] += 1
        ok = True
        for i in range(0, k):
            if flag[k] == flag[i]:
                ok = False
                break
        if not ok: continue

        if k == maxK: 
            for f in flag[:maxK]: print(colors[f], end="")
            print(colors[flag[maxK]], end=" ")
            if index % 20 == 19: print()
            index += 1
        else:
           k += 1
           flag[k] = -1
    else:
        k -= 1
