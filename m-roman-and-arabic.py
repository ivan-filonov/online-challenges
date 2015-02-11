rbase={'I':1, 'V':5, 'X':10, 'L':50, 'C':100, 'D':500, 'M':1000}
def t():
    tt='''3M1D2C
2I3I2X9V1X'''.splitlines()
    ee='''3700
-16'''.splitlines()
    for x,y in zip(tt,ee):
        ar(x)
        print(y)
def ar(x):
    x = [int(y) if y.isdigit() else rbase[y] for y in list(x)]
    for i in range (1,len(x)-1,2):
        if x[i] < x[i+2]:
            x[i] = -x[i]
    print(sum(a*b for a,b in zip(x[::2],x[1::2])))
#
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
    ar(test)

test_cases.close()
#"""
