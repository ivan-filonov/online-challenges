#"""Sample code to read in test cases:

import sys
	
def process(s):
    pass

test_cases = open(sys.argv[1], 'r')
for test in test_cases:
    test=test.strip()
    if len(test)==0:
        continue
    process(test)

test_cases.close()
