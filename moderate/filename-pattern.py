import re
def p(s):
	ss = s.split(' ')
	p,ss = ss[0], ss[1:]
	#print(p)
	for c in '\\.+{}()':
		p = p.replace(c, '\\' + c)
	p = p.replace('*','.*').replace('?', '.{1}')
	#print(p)
	p = re.compile(p)
	ss = [x for x in ss if p.match(x) and p.match(x).group(0)==x]
	print(' '.join(ss) if len(ss)>0 else '-')
#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test = test.strip();
    if len(test)==0:
        continue
    p(test)

test_cases.close()
#"""
