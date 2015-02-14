#"""Sample code to read in test cases:

import sys
morse={
	'.-':'A', '-...':'B', '-.-.':'C', '-..':'D', '.':'E', '..-.':'F',
	'--.':'G', '....':'H', '..':'I', '.---':'J', '-.-':'K', '.-..':'L',
	'--':'M','-.':'N','---':'O','.--.':'P','--.-':'Q','.-.':'R',
	'...':'S','-':'T','..-':'U','...-':'V','.--':'W','-..-':'X',
	'-.--':'Y', '--..':'Z', '.----':'1', '..---':'2', '...--':'3',
	'....-':'4','.....':'5','-....':'6','--...':'7','---..':'8',
	'----.':'9','-----':'0', }
def mor(s):
	res=[]
	while len(s)>0:
		if s[0]==' ':
			res.append(' ')
			s=s[1:]
			#print(s)
		else:
			i=s.find(' ')
			if i < 0: i = len(s)
			#print(i,s)
			c=s[:i]
			s=s[i+1:]
			res.append(morse[c])
	print(''.join(res))
test_cases = open(sys.argv[1], 'r')
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    mor(test)

test_cases.close()
#"""
