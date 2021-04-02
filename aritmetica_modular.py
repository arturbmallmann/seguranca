#!/usr/bin/python3
multiply_ar = lambda vec,n:[v * n for v in vec]
def tabuada_mod(num, mod):
	mult = [multiply_ar([x for x in range(num)],x) for x in range(num)]
	print('\n')
	for x in mult:
		linha =''
		for y in x:
				linha += f'{y%mod:3}'
		print (linha)

tabuada_mod(10,100)
tabuada_mod(10,10)
tabuada_mod(7,7)
tabuada_mod(6,6)
tabuada_mod(5,5)
tabuada_mod(19,11)
