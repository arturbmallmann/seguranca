from math import floor, sqrt
from numpy.random import randint
import sys
def crivo (n):
	l = [2]
	tp = 1
	i = 2
	while (tp < n):
		i+=1
		primo = 1
		for p in l:
			if(p > int (floor(sqrt(i)))):
				break
			if(i % p ==0):
				primo = 0
		if primo:
			tp +=1
			l+=[i]
	return l

def primdc(e,t):
#	me=[]
#	mt=[]
	for x in primos:
		if e % x == 0 and t % x == 0:
			return False
	return True

size = 340
primos = crivo(size)
p = primos[randint(size)]
q = primos[randint(size)]
while (p == q):
	q = primos[randint(size)]
t = (p-1)*(q-1)
n = p*q
msg = randint(n) # mensagem deve ser menor que N, ou seja, p*q
print(f"mensagem: {msg} p: {p} q:{q}",file=sys.stderr)
#criptografar:
e = randint(t)
while(not primdc(t,e)):
	e = randint(t)

c = pow(msg,e,n)

print (f'{n} {e} {c}',file=sys.stderr)
print (f'{n} {e} {c}')
