import math
import sys

logab = lambda a,b : math.log(a) / math.log(b)

h = lambda q,t,base: q/t * logab(q/t,base) #/ t
texto = list()


file =open(sys.argv[-1],'r+b')
pos =0
while True:
	data = file.read(1)
	if not data:
		break
	pos=pos+1
	file.seek(pos)
	#print(data, end='')
	texto += [data]

file.close()

mapa = dict()

for a in texto:
	if a in mapa:
		mapa[a] += 1
	else:
		mapa.update({a:1})

htotal = 0;
print(f"Tamanho mapa: {len(mapa)}\nTam texto: {len(texto)}")
for m in mapa:
#	print (m)
#	print (mapa[m])
	htotal -= h(mapa[m], len(texto), len(mapa))

print("Entropia: "+str(htotal))
print(mapa)
