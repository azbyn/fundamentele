from sys import argv
def find_missing(v):
    size = len(v)
    second = (v[0] + v[2]) // 2
    if second != v[1]: return v[2] - v[1] + v[0]
    ratio = v[1] - v[0]

    def impl(v):
        if len(v) == 1: return v[0] + ratio
        middle = len(v) // 2
        if v[0] + ratio * middle == v[middle]:
            return impl(v[middle:])
        return impl(v[:middle])

    res = impl(v)
    if res != v[-1] + ratio:
        return res
    return None

print("$ python3 src.py")
print("Please insert the vector:", argv[1])
v = [int(str) for str in (argv[1].split(' ')) if str != ""]
res = find_missing(v)
print("Nothing" if res == None else res, "is missing")
