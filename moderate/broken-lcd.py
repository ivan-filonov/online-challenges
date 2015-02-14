digits=[63, 6, 91, 79, 102, 109, 125, 7, 127, 111]
def blcd(x):
	lights,num=x.split(';')
	lights=[int(x[::-1],2) for x in lights.split(' ')]
	#print(num,lights)
	nums=[]
	for c in num:
		if c.isdigit():
			nums.append(digits[ord(c)-ord('0')])
		else:
			nums[-1]=nums[-1]+128
	#print(num,nums)
	for ofs in range(len(lights)-len(nums)+1):
		sl=lights[ofs:ofs+len(nums)]
		c1=[a&b for a,b in zip(nums,sl)]
		#print(' '.join(bin(x)[2:] for x in sl))
		#print(' '.join(bin(x)[2:] for x in nums))
		#print(' '.join(str(y==(x&y)) for x,y in zip(sl,nums)))
		c2=[0 if a==b else 1 for a,b in zip(c1,nums)]
		#print(nums,sl,c1,sum(c2))
		if 0 == sum(c2):
			print(1)
			return
	print(0)
#"""Sample code to read in test cases:

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
    blcd(test)

test_cases.close()
#"""
