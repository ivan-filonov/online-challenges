#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
def decr(s):
	wl, kc, s = s.split('|')
	wl,kc,s=int(wl),ord(kc.strip()),[int(x) for x in s.split()]
	diff=kc-ord(' ')
	ll=[]
	for i in range(wl,len(s)):
		if s[i-1]-s[i]==diff:
			ll.append(i-wl)
	sl={}
	for i in ll:
		d=s[i:i+wl][-1]-kc
		ss=''.join(chr(t-d) for t in s[i:i+wl])
		if ss not in sl:
			sl[ss]=[(i,d)]
		else:
			sl[ss].append((i,d))
	for k in sl:
		if len(sl[k])>=2 and sl[k][0][1]==sl[k][1][1]:
			print(''.join(chr(t-sl[k][0][1]) for t in s))
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    decr(test)

test_cases.close()
#"""
