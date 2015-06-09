#"""Sample code to read in test cases:

import sys
def sr(s):
    s1,s2=s.split(',')
    for i in range(1,len(s1)):
        if s2.count(s1[:i])==1:
            ss=s1[:i]
    if ss is None:
        print('False')
        return
    d=s2.index(ss)
    for i in range(len(s1)):
        if s1[i]!=s2[(i+d)%len(s2)]:
            print('False')
            return
    print('True')
test_cases = open(sys.argv[1], 'r')
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    sr(test)
    

test_cases.close()
#"""
