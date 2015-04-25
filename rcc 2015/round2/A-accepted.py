import sys
def line():
    return sys.stdin.readline().strip()
tests='''2 1 2
100 99 2
17 13 10
3 3 1
1 1 2'''.splitlines()

def process(line):
    a, b, k = [int(x) for x in line.split()]
    if a < b:
        a,b = b,a
    if b >= 99 and k == 1:
        return 0
    v99 = int(99 / k) + 1
    d0 = (b - v99) if b > v99 else 0
    a -= d0
    b -= d0
    for i in range(b):
        a1 = a - i
        if a1 > 99:
            a1 = 99
        b1 = b - i
        if b1 * k == a1:
            return d0 + i
    return -1

nt = int(line())
for x in range(nt):
    print(process(line()))
