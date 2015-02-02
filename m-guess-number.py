#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
def guess(s):
	parts=s.split()
	rmin,rmax=0,int(parts[0])
	for p in parts[1:]:
		mid=(rmin+rmax+1)>>1
		if 'Lower'==p: rmin, rmax = rmin, mid - 1
		elif 'Higher'==p: rmin, rmax = mid + 1, rmax
	print(mid)
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    guess(test)
test_cases.close()
#"""
