def t():
	tt='''4;1,4,2,3,2,3,1,4,4,2,3,1,3,1,4,2
4;2,1,3,2,3,2,1,4,1,4,2,3,2,3,4,1'''.splitlines()
	ee='''True
False'''.splitlines()
	for x,y in zip(tt,ee):
		sud(x)
		print(y)
def sud(x):
	N,d=x.split(';')
	N=int(N)
	d=[int(dd) for dd in d.split(',')]
	m=[d[a*N:a*N+N] for a in range(N)]
	for v in range(N):
		t,t1=[0]*N,[0]*N
		for c in m[v]: t[c-1]=1
		for h in range(N):
			t1[m[h][v]-1]=1
		if sum(t) != N or sum(t1) != N:
			print(False)
			return
	if 9==N:
		for v in range(0,9,3):
			for h in range(0,9,3):
				t=[0]*9
				for vv in range(3):
					for hh in range(3):
						t[m[v+vv][h+hh]-1]=1
				if sum(t) != 9:
					print(False)
					return
	print(True)
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
    sud(test)

test_cases.close()
#"""
