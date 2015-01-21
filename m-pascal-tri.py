#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
def step(cur):
	n=len(cur)+1
	res=[0]*n
	for i in range(n):
		res[i]=sum(cur[max(0,i-1):min(n,i+1)])
	return res
def tri(n):
	s,r=[1],[1]
	for k in range(n-1):
		s = step(s)
		r += s
	return r
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    print(' '.join(str(s) for s in tri(int(test))))

test_cases.close()
#"""
