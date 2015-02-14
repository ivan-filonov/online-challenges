def mps(n,m):
    c=[[0 for z in range(n)] for zz in range(n)]
    c[0][0] = m[0][0]
    for i in range(1, n):
        c[0][i] = c[0][i-1] + m[0][i]
        c[i][0] = c[i-1][0] + m[i][0]
    for v in range(1,n):
        for h in range(1,n):
            c[v][h] = m[v][h] + min(c[v-1][h],c[v][h-1])
    print(c[-1][-1])
def tprocess(lines):
    i = 0
    while i < len(lines):
        m,n = [], int(lines[i])
        for j in range(n):
            m.append([int(t) for t in lines[i+1+j].split(',')])
        i+=n+1
        mps(n,m)
#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
lines=[]
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    lines.append(test)

test_cases.close()
#"""

tprocess(lines)
