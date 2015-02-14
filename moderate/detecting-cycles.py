#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
def isloop(l,b,c):
	for x,y in zip(l[b:b+c],l[b+c:b+2*c]):
		if x != y: return False
	return True
def loop(x):
	x=x.split()
	for i in range(len(x)-1):
		for j in range((len(x)-i)>>1):
			if isloop(x, i, j+1):
				print(' '.join(x[i:i+j+1]))
				return

for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    loop(test)

test_cases.close()
#"""
