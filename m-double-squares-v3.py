#"""Sample code to read in test cases:
def nxt(x,x2):
	return x+1,x2+x+x+1

def pre(x,x2):
	return x-1,x2-x-x+1

def isqrt(i):
	p,x=i,1
	while abs(p-x)>0:
		p = x
		x = (x + int(i/x))>>1
#		print(p,x)
	return x

def squarez(i):
	#print('i =',i)
	res,a,b,a2,b2=0,0,0,0,0
	b = isqrt(i)
	b2 = b * b
	a = isqrt(i - b2)
	a2 = a * a
#	while b2 < i : b,b2 = nxt(b, b2)
	if b2>i: b,b2 = pre(b, b2)
	while b>=a:
		s = a2 + b2
		if s == i:
			#print(i,'=',a[0],'^2 + ', b[0],'^2')
			res += 1
			b, b2 = pre(b, b2)
			a = isqrt(i - b2) + 1
			a2 = a * a
		elif s > i: b,b2 = pre(b,b2)                        
		else:
                        while (a2+b2)<i: a, a2 = nxt(a, a2)
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
