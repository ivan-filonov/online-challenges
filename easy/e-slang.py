#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
sld=(
    ', yeah!',
    ', this is crazy, I tell ya.',
    ', can U believe this?',
    ', eh?',
    ', aw yea.',
    ', yo.',
    '? No way!',
    '. Awesome!',
    )
sldi,cc=0,0
def slang(s):
	global sldi,cc
	ii=[i for i in range(len(s)) if s[i] in '.!?']
	#print(ii)
	kk=[ii[j] for j in range(1-cc%2,len(ii),2)]
	cc+=len(ii)
	#print(kk)
	o=0
	for j in kk:
		#print(j)
		#print(s[:j])
		#print(sldi, sld[sldi])
		#print(s[j+1:])
		s=s[:j+o]+sld[sldi]+s[j+o+1:]
		o+=len(sld[sldi])-1
		sldi = (sldi+1)%len(sld)
	print(s)
	
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    slang(test)

test_cases.close()
#"""
