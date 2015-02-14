#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
def roman(i):
	rd = [(1000, 'M'),
	      (900, 'CM'),
	      (500, 'D'),
	      (400, 'CD'),
	      (100, 'C'),
	      (90, 'XC'),
	      (50, 'L'),
	      (40, 'XL'),
	      (10, 'X'),
	      (9, 'IX'),
	      (5, 'V'),
	      (4, 'IV'),
	      (1, 'I'),
	      ]
	res, i = [], int(i)
	for v,s in rd:
		while i >= v:
			res.append(s)
			i -= v
	print(''.join(res))
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    roman(test)

test_cases.close()
#"""
