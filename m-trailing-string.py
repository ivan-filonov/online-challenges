#"""Sample code to read in test cases:
def t():
	tt='''Hello World,World
Hello CodeEval,CodeEval
San Francisco,San Jose'''.splitlines()
	ee='''1
1
0'''.splitlines()
	for x,y in zip(tt,ee):
		trailing(x)
		print(y)
#t()
def trailing(x):
	a,b=[list(reversed(list(xx))) for xx in x.split(',')]
	if len(b)>len(a):
		print(0)
		return
	for c in range(len(b)):
		if a[c] != b[c]:
			print(0)
			return
	print(1)
	return
#
#t()
import sys
test_cases = open(sys.argv[1], 'r')
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    trailing(test)

test_cases.close()
#"""
