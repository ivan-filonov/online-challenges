import sys
def line():
    return sys.stdin.readline().strip()
"""
4
1
1
1
2
2
10
11
11
121
1
1
10
"""
inf, fin = 'Infinity', 'Finite'
num_tests = int(line())
for x in range(num_tests):
    xs = line()
    ys = line()
    zs = line()
    xx, yy, zz = sorted(xs), sorted(ys), sorted(zs)
    xk = 1 + int(xx[-1])
    yk = 1 + int(yy[-1])
    zk = 1 + int(zz[-1])
    k = max(xk,yk,zk)
    res = inf
    for kk in range(k, 10 + k):
        if int(xs,kk)*int(ys,kk) != int(zs,kk):
            res=fin
            break
    print(res)
    
