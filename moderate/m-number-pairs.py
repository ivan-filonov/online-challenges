#"""Sample code to read in test cases:
def npairs(x):
	a,n=x.split(';')
	n=int(n)
	a=sorted([int(aa) for aa in a.split(',')])
	i,j=0,len(a)-1
	res=[]
	while i < j:
		ai,aj=a[i],a[j]
		if ai+aj==n:
#			print(ai,aj)
			res.append((ai,aj))
			i += 1
		elif ai+aj<n: i+=1
		else: j-=1
	print('NULL' if 0==len(res) else ';'.join(','.join(str(z) for z in a) for a in res))
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
    npairs(test)

test_cases.close()
#"""