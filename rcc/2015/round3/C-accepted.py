import sys
def line():
    return sys.stdin.readline().strip()
L=0
tests='''5
2 1 2 1 25
2 3 1 1 100
2 3 5 5 11
3 9 2 4 15
3 9 2 4 13'''.splitlines()
def line2():
    global L,tests
    pl = L
    L += 1
    return tests[pl]

def process(line):
    a1,b1,a2,b2,z=[int(x) for x in line.split()]
    h1,h2=0,0
    t = 0
    r = 0
    while t < z:#!!! < <=
        if t % 24 < 12:
            d1, d2 = a1, a2
        else:
            d1, d2 = -b1, -b2
        j1, j2 = h1 + d1, h2 + d2
        #...
        sd = (h1 - h2) * (j1 - j2)
        if 0 == sd:
            # пересеклись
            if h1 > h2 or j1 > j2:
                r += 1
        elif sd > 0:
            # без изменений
            if h1 > h2:
                r += 1
        else:
            #изменение
            k = abs((h2 - h1)/(d2 - d1))
            r += k if h1 > h2 else (1 - k)
        #...
        h1, h2 = j1, j2
        t += 1
    return r

nt = int(line())
for x in range(nt):
    print(process(line()))
