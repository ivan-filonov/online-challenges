#"""Sample code to read in test cases:
def nxt(pair):
	return (pair[0]+1,pair[1]+2*pair[0]+1)
def pre(pair):
	return (pair[0]-1, pair[1]-2*pair[0]+1)
def squarez(i):
	#print('i =',i)
	res,a,b=0,(0,0),(0,0)
	while b[1] < i :
		b = nxt(b)
	if b[1]>i: b = pre(b)
	while b[0]>=a[0]:
		s = a[1] + b[1]
		if s == i:
			#print(i,'=',a[0],'^2 + ', b[0],'^2')
			res += 1
			a = nxt(a)
		elif s > i: b = pre(b)
		else: a = nxt(a)
	print(res)

import sys
test_cases = open(sys.argv[1], 'r')
nc = None
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    if nc is None:
        nc = int(test)
    else:
        squarez(int(test))
        nc -= 1
        if 0 == nc:
            break

test_cases.close()
#"""
