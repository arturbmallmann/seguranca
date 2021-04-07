import math
import sys

from math import floor, sqrt
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
#primos = crivo(3500)
#print(primos)

def euclides (a, b):
	r = a
	r1 = b
	u = 1
	v = 0
	u1 = 0
	v1 = 1
# variáveis auxiliares para efetuar trocas
# rs, us, vs, q

	while (r1 != 0):
		q = int (r / r1) # pega apenas a parte inteira
		rs = r
		us = u
		vs = v
		r = r1
		u = u1
		v = v1
		r1 = rs - q *r1
		u1 = us - q*u
		v1 = vs - q*v1

	return [r, u, v] # tais que a*u + b*v = r et r = pgcd (a, b)

values = input()
(n,e,c) = tuple([int(x) for x in values.split(' ')])
p, q = (0,0)
ciclos = 0
for x in range(3,math.ceil(math.sqrt(n)),2):
#for x in primos:
#	ciclos += 1
	if n % x == 0 :
#		print(f'multiplos: {x} * {n/x} = {n}')
		p = int(n/x)
		q = x
		break

#print(f'ciclos {ciclos}')
print (f'p {p} q {q}',file=sys.stderr)
totiente = (p-1)*(q-1)
#print(f'totiente: {totiente}')
#inverso
# d * e = 1 mod totiente
#for d in range(e*2,totiente,totiente%e):

#print(f'ciclos: {ciclos}')
(r,u,v)=euclides(e,totiente)
#print(f'u/t: {(r,u,v)}')
#msg = c**d % n
print(pow(c,u,n))
#msg = pow(c, d, n)
#print (msg)
