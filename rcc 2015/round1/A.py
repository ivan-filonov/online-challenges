import sys
def line():
    return sys.stdin.readline().strip()
num_tests = int(line())
for x in range(num_tests):
    n,l=[int(s) for s in line().split()]
    gri = [int (s) for s in line().split()]
    dim = [int (s) for s in line().split()]
    gri.sort()
    dim.sort()
    if n > l :
        sg, sd = sum(gri[:-(n-l)]), sum(dim[n-l:])
    else:
        sg, sd = sum(gri), sum(dim)
    print('YES' if sg > sd else 'NO')
