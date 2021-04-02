import math
values = input()
(n,e,c) = tuple([int(x) for x in values.split(' ')])
p, q = (0,0)
ciclos = 0
for x in range(3,math.ceil(math.sqrt(n)),2):
#for x in a:
	ciclos += 1
	if n % x == 0 :
#		print(f'multiplos: {x} * {n/x} = {n}')
		p = int(n/x)
		q = x
		break

#print(f'ciclos {ciclos}')
print (f'p {p} q {q}')
totiente = (p-1)*(q-1)
#print(f'totiente: {totiente}')
#inverso
# d * e = 1 mod totiente
#for d in range(e*2,totiente,totiente%e):
ciclos = 0
for d in range(0,totiente,e):
	ciclos +=1
#	print (d)
	if d*e % totiente == 1:
#		print (f'd: {d}')
		break
#print(f'ciclos: {ciclos}')
print(f'c: {c} d:{d}')
msg = c**d % n
print (msg)
