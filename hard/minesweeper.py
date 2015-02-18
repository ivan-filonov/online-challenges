import sys
test_cases = open(sys.argv[1], 'r')
def mines(s):
	mn,s=s.split(';')
	rows,cols=(int(x) for x in mn.split(','))
	s=list(s)
	def test(x,y):
		if x<0 or y<0 or x>=cols or y>=rows:
			return 0
		return 1 if s[y*cols+x]=='*' else 0
	for h in range(cols):
		for v in range(rows):
			if '*'==s[v*cols+h]:
				continue
			s[v*cols+h]=str(sum(test(h+dh,v+dv) for dh in range(-1,2) for dv in range(-1,2)))
	print(''.join(s))
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    mines(test)

test_cases.close()
