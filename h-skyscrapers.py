def outline(x):
	x=[[int(u) for u in xx.strip()[1:-1].strip().split(',')] for xx in x.split(';')]
	#print(x)
	h = [0]*(max(x, key=lambda t:t[2])[2]+1)
	for ll,hh,rr in x:
		for i in range(ll,rr):
			h[i] = max(h[i], hh)
	#print(h)
	ch,res = 0,[]
	for ci in range(len(h)):
		if ch==h[ci]: continue
		ch = h[ci]
		#print(ci, ch)
		res.append(ci)
		res.append(ch)
	print(' '.join(str(x) for x in res))
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
    outline(test)

test_cases.close()
#"""
