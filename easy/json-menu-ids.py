#"""Sample code to read in test cases:
def mis(s):
	cs_id, cs_l ='"id": ', ', "label": '
	i,r = s.find(cs_id),0
	while i > -1:
		i += len(cs_id)
		i1 = i
		while s[i1].isdigit():	i1 += 1
		if s[i1:].startswith(cs_l):
			r += int(s[i:i1])
		i = s.find(cs_id, i)
	print(r)

def t():
    tt='''{"menu": {"header": "menu", "items": [{"id": 27}, {"id": 0, "label": "Label 0"}, null, {"id": 93}, {"id": 85}, {"id": 54}, null, {"id": 46, "label": "Label 46"}]}}
{"menu": {"header": "menu", "items": [{"id": 81}]}}
{"menu": {"header": "menu", "items": [{"id": 70, "label": "Label 70"}, {"id": 85, "label": "Label 85"}, {"id": 93, "label": "Label 93"}, {"id": 2}]}}'''.splitlines()
    ee='''46
0
248'''.splitlines()
    for x,y in zip(tt,ee):
        mis(x)
        print(y)

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
    mis(test)

test_cases.close()
#"""
