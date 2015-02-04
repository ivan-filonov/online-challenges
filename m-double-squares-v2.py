#"""Sample code to read in test cases:
def nxt(x,x2):
	return x+1,x2+x+x+1

def pre(x,x2):
	return x-1,x2-x-x+1

def squarez(i):
	#print('i =',i)
	res,a,b,a2,b2=0,0,0,0,0
	while b2 < i : b,b2 = nxt(b, b2)
	if b2>i: b,b2 = pre(b, b2)
	while b>=a:
		s = a2 + b2
		if s == i:
			#print(i,'=',a[0],'^2 + ', b[0],'^2')
			res += 1
			a,a2 = nxt(a,a2)
		elif s > i: b,b2 = pre(b,b2)
		else: a,a2 = nxt(a,a2)
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
