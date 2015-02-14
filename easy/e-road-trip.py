#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
def rtrip(s):
	parts=[int(y) for x,y in [s.strip().split(',') for s in [x for x in s.split(';') if len(x)>1]]]
	parts.sort()
	for i in range(len(parts)-1,0,-1):
		parts[i] -= parts[i-1]
	print(','.join(str(x) for x in parts))
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    rtrip(test)

test_cases.close()
#"""
