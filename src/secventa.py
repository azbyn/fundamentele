import sys
print("Please insert the sequence: "+sys.argv[1])
strs = sys.argv[1].split(' ')
v = [int(num) for num in strs]
n = len(v)
# python way of defining a n-dimensional list initialized to 0
sub_sums = [0 for i in range(0, n)]

best = (0, 0)
best_sum = 0
for i in range(0, n):
    sub_sums[i] = v[i]
    best_end_index = i
    # after this loop v[j] = (sum from k=i to j of v[k])
    for j in range(i+1, n):
        sub_sums[j] = sub_sums[j-1] + v[j]
        if sub_sums[j] > sub_sums[best_end_index]:
            best_end_index = j
    if sub_sums[best_end_index] > best_sum:
        best_sum = sub_sums[best_end_index]
        best = (i, best_end_index)

print("Best with a sum of", best_sum, "is: (x%d,...,x%d)" % best)
