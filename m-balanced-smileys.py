#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
def bal(s):
	prev,d,ml,mr=None,0,0,0
	for c in s:
		if '('==c:
			if ':' == prev: ml += 1
			else: d += 1
		elif ')'==c:
			if ':' == prev:
				mr += 1
			else:
				if d > 0: d -= 1
				else:
					if ml > 0: ml -= 1
					else:
						d=mr+1
						break
		prev = c
	# comment
	print('YES' if d<=mr else 'NO')
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    bal(test)

test_cases.close()
#"""
