class Interval:
    def __init__(self, a, b):
        self.a = a
        self.b = b
    def __str__(self): return "[%d, %d]" % (self.a, self.b)
    def __repr__(self): return "[%d, %d]" % (self.a, self.b)

    def intersection(A, B):# $A \cap B$
          # assume A is on the left
          if A.a > B.b: return B.intersection(A)
          if A.b < B.a: return None
          return Interval(B.a, A.b)

    def in_(A, B): # $A \subseteq B$
        return A.a >= B.a and A.b <= B.b
    def __eq__(A, B): #$A = B$
        return A.a == B.a and A.b == B.b
    def length(self): return self.b - self.a
    def dup(self): return Interval(self.a, self.b)

class SuperInterval:
    def __init__(self, interval):
        self.intervals = [interval.dup()]

    # returns True if it can be eliminated
    def add_reunion(self, A):
        # we could sort the intervals after b, but this is already
        # a lot of code

        for i in range(len(self.intervals)):
            itv = self.intervals[i]
            intr = A.intersection(itv)
            if intr is None: continue
            if A.in_(itv): return True
            if itv.in_(A):
                itv.a = A.a
                itv.b = A.b
                return True
            if A.a < itv.a: itv.a = A.a
            if A.b > itv.b: itv.b = A.b
            itv = self.intervals.pop(i)
            return self.add_reunion(itv)
        self.intervals.append(A.dup())
        return False
    # $\mathit{self} \cap A$ if not $\varnothing$, $\mathit{self} \cup A$ otherwise
    def add_intersection(self, A):
        for itv in self.intervals:
            intr = A.intersection(itv)
            if intr is None: continue
            itv.a = intr.a
            itv.b = intr.b
            return

        self.intervals.append(A.dup())

    def length(self):
        l = 0
        for i in self.intervals: l += i.length()
        return l
    def __str__(self):
        s = ""
        for i in self.intervals: s += str(i) + " "
        return s


print("$ python3 intervals.py")
from sys import argv
print("Please insert intervals (of format [a,b]):", argv[1])
strs = argv[1].split(' ')
from parse import parse
intervals = []
for s in strs:
    res = parse("[{},{}]", s)
    intervals.append(Interval(int(res[0]), int(res[1])))


si = SuperInterval(intervals[0])
si2 = SuperInterval(intervals[0])
count = 0
for i in intervals[1:]:
    if si.add_reunion(i): count += 1
    si2.add_intersection(i)

X = [i.a for i in si2.intervals]
print("a)", si.length())
print("b)", si)
print("c)", count)
print("d) X = {", ("%s" % X)[1:-1], "}")
