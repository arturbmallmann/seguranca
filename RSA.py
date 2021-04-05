import math

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
primos = crivo(3500)
#print(primos)

values = input()
(n,e,c) = tuple([int(x) for x in values.split(' ')])
p, q = (0,0)
ciclos = 0
#for x in range(3,math.ceil(math.sqrt(n)),2):
for x in primos:
	ciclos += 1
	if n % x == 0 :
#		print(f'multiplos: {x} * {n/x} = {n}')
		p = int(n/x)
		q = x
		break

#print(f'ciclos {ciclos}')
#print (f'p {p} q {q}')
totiente = (p-1)*(q-1)
#print(f'totiente: {totiente}')
#inverso
# d * e = 1 mod totiente
#for d in range(e*2,totiente,totiente%e):
ciclos = 0
for d in range(math.floor(math.sqrt(p*q)),totiente,1):
	ciclos +=1
#	print (d)
	if d*e % totiente == 1:
#		print (f'd: {d}')
		break
#print(f'ciclos: {ciclos}')
#print(f'c: {c} d:{d}')
#msg = c**d % n
msg = pow(c, d, n)
print (msg)
