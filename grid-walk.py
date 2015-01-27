def test(x,y):
	q=0
	for z in (x,y):
		z=abs(z)
		while z > 0:
			q,z=q+z%10,int(z/10)
	return q <= 19
points, ps=set(), [(0,0)]
def fill(x,y):
	global points,ps
	if (x*10000+y) in points:
		return
	if not test(x,y):
		return
	points.add(x*10000+y)
	ps = ps + [(x+d*s,y+d*(1-s)) for d in (1,-1) for s in (0,1)]
while len(ps)>0:
    fill(*ps.pop())
print(len(points))
