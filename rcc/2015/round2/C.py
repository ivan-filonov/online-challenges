import sys
def line():
    return sys.stdin.readline().strip()
tests='''3
1 2 3
1
4 
5
5 5 5 5 5'''.splitlines()

def p(n,l):
    n = int(n)
    l = [int(x) for x in l.split()]
    mv, mi = l[0], 0
    for z,v in enumerate(l):
        if v > mv:
            mv, mi = v, z
    s = sum(l[:mi])
    return max(mv / 2, mv - s)
    
nt = int(line())
for x in range(nt):
    ln = line()
    la = line()
    v = p(ln, la)
    print(v)
