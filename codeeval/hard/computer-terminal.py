#"""Sample code to read in test cases:

import sys
cx,cy,vm,ins=0,0,[' '*10 for i in range(10)],0
def show():	print('\n'.join('"'+s+'"' for s in vm))
def t():
	global cx,cy,vm
	cx,cy,vm=0,0,[' '*10 for i in range(10)]
	for s in r'''^h^c
^04^^
^13/ \^d^b  /   \
^u^d^d^l^l^l^l^l^l^l^l^l
^r^r^l^l^d<CodeEval >^l^l^d/^b \
^d^r^r^66/^b  \
^b^d   \ /
^d^l^lv^d^b===========^i^94O123456
789^94A=======^u^u^u^u^u^u^l^l\^o^b^r/'''.splitlines():
		process(s)
	show()
	print(r'''-----------------------------
    ^
   / \
  /   \
 /     \
<CodeEval>
 \     /
  \   /
   \ /
    v
====A=====''')
	
def process(s):
	global cx,cy,vm,ins
	cmd,cmd2,d0=0,0,0
	for c in s:
		#print(cmd,cmd2,d0,ins,c)
		if cmd2:
			cmd2 = 0
			cx,cy=int(c),d0
			continue
		if not cmd and '^'==c:
			cmd=1
			continue
		if cmd:
			if 'c'==c: vm=[' '*10 for i in range(10)]
			elif 'h'==c: cx,cy=0,0
			elif 'b'==c: cx=0
			elif 'd'==c: cy=min(9,cy+1)
			elif 'u'==c: cy=max(0,cy-1)
			elif 'l'==c: cx=max(0,cx-1)
			elif 'r'==c: cx=min(9,cx+1)
			elif 'e'==c: vm[cy]=vm[cy][:cx] + ' '*(9-cx)
			elif 'i'==c: ins=1
			elif 'o'==c: ins=0
			elif c.isdigit(): cmd2,d0 = 1,int(c)
			cmd = 0
			if '^' != c:
				continue
		vm[cy]=(vm[cy][:cx]+c+vm[cy][cx+(1-ins):])[0:10]
		if cx < 9: cx += 1

test_cases = open(sys.argv[1], 'r')
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    process(test)

for s in vm: print(s)

test_cases.close()
#"""
