#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
def chain(s):
    pairs=[p.strip().split('-') for p in s.split(';')]
    d={}
    for x,y in pairs:
        if x in d:
            print('BAD')
            return
        d[x.strip()]=y.strip()
    i='BEGIN'
    for j in range(len(d)):
        if i not in d:
            print('BAD')
            return
        i = d[i]
    print('GOOD' if i=='END' else 'BAD')
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    chain(test)

test_cases.close()
#"""
