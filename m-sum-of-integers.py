def t():
    tt='''-10,2,3,-2,0,5,-15
2,3,-2,-1,10'''.splitlines()
    ee=['8','12']
    for x,y in zip(tt,ee):
        msum(x)
        print(y)

def msum(x):
    x = [int(xx) for xx in x.split(',')]
    msum = x[0]
    for i in range(len(x)):
        csum = 0
        for j in range(len(x[i:])):
            csum += x[i+j]
            msum = max(msum, csum)
    print(msum)

#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue

    msum(test)

test_cases.close()
#"""
