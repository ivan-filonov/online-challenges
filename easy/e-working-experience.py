#"""Sample code to read in test cases:

import sys
months=['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
def d2n(s):
	m,y=s.split()
	m,y=months.index(m),int(y)
	return m + 12 * y
def cm(s):
	d={}
	ss=s.split('; ')
	for z in ss:
		d1,d2=[d2n(x) for x in z.split('-')]
		for dd in range(d1,d2+1):
			d[dd]=1
	print(int(len(d)/12))
test_cases = open(sys.argv[1], 'r')
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    cm(test)

test_cases.close()
#"""
