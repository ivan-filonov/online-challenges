#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
def spiral(s):
	h,w,m=s.split(';')
	h,w,m=int(h),int(w),m.split()
	m=[[m[w*r+x] for x in range(w)] for r in range(h)]
	#print(h,w,m)
	r=[]
	def p_top():
		return m[1:],r+m[0]
	def p_bottom():
		return m[:-1],r+list(reversed(m[-1]))
	def p_right():
		return [x[:-1] for x in m],r+[x[-1] for x in m]
	def p_left():
		return [x[1:] for x in m],r+list(reversed([x[0] for x in m]))
	def ms():
		return sum(len(x) for x in m)
	i,ops=0,(p_top,p_right,p_bottom,p_left)
	while ms()>0:
		m,r=ops[i%len(ops)]()
		i+=1
	print(' '.join(r))

for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    spiral(test)

test_cases.close()
#"""
