import sys
def line1():
    return sys.stdin.readline().strip()
L=0
tests='''3 1
111
1 110'''.splitlines()
tests2='''4 3
1011
1 1111
1 1010
0 0110'''.splitlines()
def line():
    global L,tests
    pl = L
    L += 1
    return tests[pl]

