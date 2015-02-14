def test(x,y):
	return ds(x)+ds(y)
def ds(n):
	n,s = abs(n),0
	while n > 0:
		n,s = int(n/10), s+n%10
	return s
def test(x,y):
	return ds(x)+ds(y) <= 19
def idx(x,y):
	return (x+300)*600+(y+300)
def fill_slow(x0,y0):
	points=[0]*(600*600)
	st=[(x0,y0)]
	def fill():
		x,y=st.pop()
		if points[idx(x,y)]: return
		if not test(x,y): return
		points[idx(x,y)]=1
		st.append((x+1,y))
		st.append((x,y+1))
		st.append((x-1,y))
		st.append((x,y-1))
	while len(st) > 0:
		fill()
	return sum(points)
print(fill_slow(0,0))
