#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
def places(s):
	idx = [i for i in range(len(s)) if 'C' == s[i]] + [i for i in range(len(s)) if '_' == s[i]]
	return idx
def process(tr):
	p = places(tr[0])[0]
	for si in range(len(tr)):
		s = tr[si]
		#print(s)
		for i in places(s):
			if abs(p-i)<2:
				j = i
				break
		s=s[:j]+'/|\\'[j-p+1]+s[j+1:]
		p = j
		print(s)
w=[]
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    w.append(test)
process(w)

test_cases.close()
#"""
