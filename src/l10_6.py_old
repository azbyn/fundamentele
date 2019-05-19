def get_sequence_len(s):
    i = 1
    for i in range(1, len(s)):
        if i % 2 == 1:
            if s[i-1] > s[i]: return i
        else:
            if s[i-1] < s[i]: return i
    return i + 1

#s = [2, 0, 2, 0, 2, 2, 0, 1, 0, 1, 1]
#s = [1,2,3,4,5,6,7]


from sys import argv
print("$ python3 src.py")
print("Please insert the set:", argv[1])
strs = argv[1].split(' ')
s = [int(str) for str in strs if str != ""]
max_i = 0
max_l = 1
i = 0
n = len(s)
while i < n:
    l = get_sequence_len(s[i:])
    if l > max_l:
        max_l = l
        max_i = i
    i += l

print("m:", s[max_i:max_i+max_l])
