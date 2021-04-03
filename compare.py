from time import time_ns
b=422 
e=71
m=1073
a = time_ns()
r1 = b ** 71 % m
print(time_ns() - a)

a = time_ns()
r2 = (( b **14 %m) ** 5 %m)  * (b ** 1 % m) % m
print(time_ns() - a)

a = time_ns()
r3 = pow(b,71,m)
print(time_ns() - a)

print(f'r1 {r1} r2 {r2} r3 {r3}')
