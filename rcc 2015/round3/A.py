import sys
def line():
    return sys.stdin.readline().strip()
tests='''4
1 2 4
2 1 4
1 2 3
3 1 6'''.splitlines()

def process(line):
    a,b,c=[int(x) for x in line.split()]
    if b * 2 >= c:
        return 'YES' if a >= c % 2 else 'NO'
    c -= b * 2
    return 'YES' if c <= a else 'NO'

nt = int(line())
for x in range(nt):
    print(process(line()))
