from sys import argv
print("$ python3 src.py")
print("Please insert s1:", argv[1])
print("Please insert s2:", argv[2])
s1 = argv[1]
s2 = argv[2]
#a dictionary of type { subsequences : is_in_s2},
cache = {}

def contains_seq(seq):
    def impl():
        for i in range(len(s2) - len(seq)+1):
            for j in range(len(seq)):
                if s2[i+j] != seq[j]: break
            else:
                # this gets called if the for didn't break
                return True
        return False

    if seq not in cache:
        cache[seq] = impl()
    return cache[seq]

best_ss = None
def check_subsequences(str_, res="", i=0):
    global best_ss
    #print("call(%s, %s, %s)" % (string, res, i))
    if i >= len(str_): return

    check_subsequences(str_, res, i+1)

    for j in range(i, len(str_)):
        ss = res + str_[j]
        if not contains_seq(ss):
            if best_ss is None or len(ss) < len(best_ss):
                best_ss = ss
        check_subsequences(str_, ss, j+1)

check_subsequences(s1)
if best_ss is not None:
    print("Best sequence: '%s'" % best_ss)
else: print("No such sequence found")
