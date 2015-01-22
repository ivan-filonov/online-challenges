#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
def digits(s):
	d,r=dict(),list()
	for i in range(10):
		d[str(i)]=str(i)
		d[chr(i+ord('a'))]=str(i)
	for c in s:
		if c in d:
			r.append(d[c])
	print(''.join(r) if len(r)>0 else 'NONE')

for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    digits(test)

test_cases.close()
#"""
