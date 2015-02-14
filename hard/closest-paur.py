#"""Sample code to read in test cases:

import sys, math
test_cases = open(sys.argv[1], 'r')
counter, points, dist=0, [], []
def line(s):
	global counter, points, dist
	if counter==0:
		dist.sort()
		points,counter=[],int(s)
		if len(dist)>0:
			d,dist=dist[0],[]
			print('%.4f'%d if d < 10000 else 'INFINITY')
	else:
		x,y=[int(t) for t in s.split()]
		dist += [math.sqrt((x-pt[0])*(x-pt[0])+(y-pt[1])*(y-pt[1])) for pt in points]
		points.append((x,y))
		counter-=1
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    line(test)

test_cases.close()
#"""
