#"""Sample code to read in test cases:
tree = [30, 8, 52, 3, 20, -1, -1, -1, -1, 10, 29]
def p(x):
    res, i = [x], tree.index(x)
    while i > 0:
        i = (i-1)>>1
        res.append(tree[i])
    return res
def lca(s):
    v1,v2=[int(ss) for ss in s.split()]
#    print(v1,v2)
    p1,p2=p(v1),p(v2)
#    print(p1,p2)
    p1,p2=list(reversed(p1)), list(reversed(p2))
    i = 0
    while i < len(p1) and i < len(p2):
        if p1[i] == p2[i]:
            i+=1
        else:
            break
    print(p1[i-1])
def t():
    for x,y in zip(('8 52', '3 29'), ('30','8')):
        lca(x)
        print(y)
#t()

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
    lca(test)

test_cases.close()
#"""
