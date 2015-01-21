#--coding:utf8--
"""
1   ()       Brackets
2   -        Unary minus
3   ^        Exponent
4   *, /     Multiply, Divide (left-to-right precedence)
5   +, -     Add, Subtract (left-to-right precedence)

test:
250*14.3
3^6 / 117
(2.16 - 48.34)^-1
(59 - 15 + 3*6)/21
res:
3575
6.23077
−0.02165
2.95238
Constraints:
Each number in input expression is greater than -20,000 and less than 20,000.
Each output number is greater than -20,000 and less than 20,000.
If output number is a float number it should be rounded to the 5th digit after the dot.
E.g 14.132646 gets 14.13265, 14.132644 gets 14.13264, 14.132645 gets 14.13265.

If output number has less than 5 digits after the dot you don't need to add zeros.
E.g. you need to print 16.34 (and not 16.34000) in case the answer is 16.34.
And you need to print 16 (and not 16.00000) in case the answer is 16.
"""
def gettok(s,pos):
  while s[pos].isspace():
      pos+=1
  if s[pos] in '+-()*^/':
    return s[pos],pos+1
  try:
    l=1
    while pos+l<len(s):
      float(s[pos:pos+l+1])
      l += 1
  except:
    return float(s[pos:pos+l]),pos+l
  return float(s[pos:]),len(s)

def tokenize(s):
  res=[]
  pos = 0
  while pos < len(s):
    tok,pos=gettok(s,pos)
    res.append(tok)
  return res

"""
expr:  expr1 ('+'|'-' expr1)+
expr1: expr2 ('*'|'/' expr2)+
expr2: expr3 ('^' expr3)+
expr3: '-' expr4 | expr4
expr4: '(' expr ')' | value
"""

class node(object):
    def __init__(self,tok):
        self.tok=tok
    def _deco(f):
        def m(s):
            r = f(s)
            print('val() for ',str(s),'->',r)
            return r
        return f
    @_deco
    def val(self):
        return float(self.tok)
    def __str__(self):
        return '[{0}]'.format(self.tok)

ops1={'-':lambda x:-x}

class n_unop(node):
    def __init__(self,op,r):
        self.r=r
        node.__init__(self,op)
    @node._deco
    def val(self):
        return ops1[self.tok](self.r.val())
    def __str__(self):
        return '[{0} {1}]'.format(self.tok,str(self.r))

def ppow(x,y):
    try:
        return pow(x,y)
    except Exception as e:
        return pow(int(x),int(y))

ops2={
    '+':lambda x,y:x+y,
    '-':lambda x,y:x-y,
    '*':lambda x,y:x*y,
    '/':lambda x,y:x/y,
    '^':lambda x,y:ppow(x,y),
    }

class n_binop(node):
    def __init__(self,l,op,r):
        self.l,self.r=l,r
        #super(node,self).__init__(op)
        node.__init__(self,op)
    @node._deco
    def val(self):
        return ops2[self.tok](self.l.val(), self.r.val())
    def __str__(self):
        return '[{0} {2}, {1}]'.format(self.tok,str(self.r),str(self.l))

def expr(t):
    l,t=expr1(t)
    while len(t)>0:
        op=t[0]
        if op == '-' or op == '+':
            r,t=expr1(t[1:])
            l=n_binop(l,op,r)
        else:
            break
    return l,t

def expr1(t):
    l,t=expr2(t)
    while len(t)>0:
        op=t[0]
        if op == '*' or op =='/':
            r,t=expr2(t[1:])
            l=n_binop(l,op,r)
        else:
            break
    return l,t

def expr2(t):
    l,t=expr3(t)
    while len(t)>0:
        op=t[0]
        if op == '^':
            r,t=expr3(t[1:])
            l=n_binop(l,op,r)
        else:
            break
    return l,t

def expr3(t):
    op=t[0]
    if op=='-':
        r,t=expr4(t[1:])
        return n_unop(op,r),t
    return expr4(t)

def expr4(t):
    if t[0]!='(':
        n=node(t[0])
        return n,t[1:]
    else:
        n,t=expr(t[1:])
        if t[0] != ')':
            raise Exception('expected ), t = ' + str(t))
        return n,t[1:]

def main(s):
    ast,rest=expr(tokenize(s))
    v=round(ast.val(),5)
    if v==int(v): v=int(v)
    #print(s,'->',ast,'-->',v,'expected',e)
    print(v)

if __name__=='__main__':
  for s,e in zip("""250*14.3
3^6 / 117
(2.16 - 48.34)^-1
(59 - 15 + 3*6)/21""".split('\n'),"""3575
6.23077
−0.02165
2.95238""".split('\n')):
    main(s)
