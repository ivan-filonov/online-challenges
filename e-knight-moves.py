#"""Sample code to read in test cases:

import sys
moves = [(x*sx,y*sy) for x,y in ((1,2),(2,1)) for sx in (-1,1) for sy in (-1,1)]
def km(s):
	v,h=ord(s[0])-ord('a'),ord(s[1])-ord('1')
	#print(s,'=',v,h)
	res=[]
	for mx,my in moves:
		pv,ph=v+mx,h+my
		#print(v,h,'+',mx,my,'->',pv,ph,'or',v+mx,h+my)
		if pv not in range(8) or ph not in range(8):
			continue
		res.append(chr(pv+ord('a'))+chr(ph+ord('1')))
	print(' '.join(sorted(res)))

test_cases = open(sys.argv[1], 'r')
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    km(test)

test_cases.close()
#"""
