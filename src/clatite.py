import sys
# index 0 represents the topmost pancake
print("Please insert a space separated list of pancake diameters:\n("+sys.argv[1])
str = sys.argv[1].split(' ')
pancakes = [int(num) for num in reversed(str)]
flips = 0

def draw(msg, vec, spatulaIndex = -1):
    print(msg+ "(", end="")
    i = len(vec)
    if spatulaIndex > 0:
        for i in range(i-1, spatulaIndex-1, -1): print(vec[i], end=" ")
        print("|", end="")
        
    for i in range(i-1, -1, -1): print(vec[i], end=" ")
    print()

def flip(vec, index):
    global flips
    flips += 1
    draw("flip %2d: " %flips, pancakes, index)
    for i in range(0, index//2):
        t = vec[i]
        vec[i] = vec[index-i-1]
        vec[index-i-1] = t

for bottom in range(len(pancakes), 0, -1):
    spatulaIndex = 0
    for i in range(0, bottom):
        if pancakes[i] >= pancakes[spatulaIndex]: spatulaIndex = i
    spatulaIndex += 1
    # if the biggest is at the bottom we do nothing
    if spatulaIndex == bottom: continue
    # if the biggest is already at the top we don't have to flip it
    if spatulaIndex != 1: flip(pancakes, spatulaIndex)
    
    flip(pancakes, bottom)

draw("", pancakes)
print("Done in %d flips" % flips)
