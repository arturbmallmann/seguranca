#!/usr/bin/python3

H=lambda x, base,n: -x*log(x/n,base)/n

t_in = input("entre o texto:\n")

in_list = [x for x in t_in]
in_list.sort()



print(in_list)
