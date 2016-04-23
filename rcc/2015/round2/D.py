import sys
def line():
    return sys.stdin.readline().strip()

mod = 1000000023

fibb = [0, 1, 1, 2]
def fib(n):
    while len(fibb) <= n:
        f1 = fibb[-1]
        f2 = fibb[-2]
        fibb.append((f1 + f2) % mod)
    return fibb[n]

pwd = dict()
def pw(n,k):
    if n in pwd:
        return pwd[n]
    s = n
    r = 1
    while k > 0:
        if k % 2 == 1:
            r = (r * s) % mod
        k = int(k/2)
        s = (s * s) % mod
    pwd[n] = r
    return r

def p(l):
    global pwd
    pwd = dict()
    n, k = [int(x) for x in l.split()]
    r = 0
    for i in range(n + 1):
        r = (r + pw(fib(i), k)) % mod
    return r

nt = int(line())
for x in range(nt):
    print(p(line()))
