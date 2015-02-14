#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
human='This program is for humans'
res=['Still in Mama\'s arms'] * 3 + ['Preschool Maniac'] * 2 +  ['Elementary school'] * 7 +  ['Middle school' ] * 3 +    ['High school' ] * 4 +    ['College'] * 4 +    ['Working for the man' ] * 43 +    ['The Golden Years' ] * 35
for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    i=int(test)
    print(res[i] if 0 <= i <= 100 else human)
    
test_cases.close()
#"""
