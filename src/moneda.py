import sys, random, math
# The values are not important, what matters is that fakeWeight < realWeight
realWeight = 40
fakeWeight = 35

length = 15 # input

coins = [realWeight] * length
#coins[random.randrange(length)] = fakeWeight
coins[12] = fakeWeight

bigPile = coins
steps = 0

while len(bigPile) > 1:
    steps += 1
    print ("Step %d:" % steps)
    #we split the big pile in 3 piles with the same size ±1
    pileSize = len(bigPile) // 3 + (len(bigPile) % 3 == 2)
    leftPile = bigPile[0:pileSize]
    rightPile = bigPile[pileSize:pileSize*2]
    leftoverPile = bigPile[pileSize*2:]
    print("left", leftPile)
    print("right", rightPile)
    print("leftover", leftoverPile)
    difference = 0
    for i in range(pileSize):
        difference += rightPile[i] - leftPile[i]

    if difference > 0: bigPile = leftPile
    elif difference < 0: bigPile = rightPile
    else: bigPile = leftoverPile
    print()

print("Took", steps, "steps to find the fake coin, expected", 
      math.ceil(math.log(length, 3)))
