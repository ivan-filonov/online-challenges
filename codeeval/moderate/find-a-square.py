#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
def square(s):
	def pt(s):
		brl, brr=s.find('(')+1, s.find(')')
		#print(s[brl:brr], '|', s[brr+1:])
		return [int(x) for x in s[brl:brr].split(',')]+[s[brr+1:]]
	x,y=[0]*4,[0]*4
	for i in range(4):
		x[i],y[i],s=pt(s)
	for i in range(3):
		x[i+1],y[i+1]=x[i+1]-x[0],y[i+1]-y[0]
	x,y,p=x[1:],y[1:],[0]*3
	for m in range(3):
		p[m]=x[(m+1)%3]*x[(m+2)%3]+y[(m+1)%3]*y[(m+2)%3]
	if p.count(0)!=1:
		print('false')
		return
	zi=p.index(0)
	if x[zi]==x[(zi+1)%3]+x[(zi+2)%3] and x[zi]==x[(zi+1)%3]+x[(zi+2)%3]:
		print('true')
	else:	print('false')

for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    square(test)

test_cases.close()
#"""
