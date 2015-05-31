import sys
def line():
    return sys.stdin.readline().strip()
L=0
tests='''4
1 5
7 7
9 12
12 22'''.splitlines()
def line1():
    global L,tests
    pl = L
    L += 1
    return tests[pl]

def z(n):
    if n < 10:
        return n
    s = 0
    while n > 0:
        s += n % 10
        n = int(n/10)
    return z(s)

def process(line):
    a,b=(int(x) for x in line.split())
    c = [0] * 10
    if b - a < 30:
        for x in range(a,b+1):
            c[z(x)] += 1
        d = [x for x in range(10) if c[x] == max(c)]
        return str(len(d)) + ' ' + ' '.join(str(x) for x in d)
    a1,b1 = a,b
    for x in range(z(a1),10):
        c[x] += 1
    for x in range(1,z(b1)+1):
        c[x] += 1;
    d = [x for x in range(10) if c[x] == max(c)]
    if len(d) > 0:
        return str(len(d)) + ' ' + ' '.join(str(x) for x in d)
    else:
        return '9 ' + ' '.join(str(x) for x in range(1,10))

nt = int(line())
for x in range(nt):
    print(process(line()))

#...................................................
