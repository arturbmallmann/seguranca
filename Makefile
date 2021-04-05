CC=g++
CFLAGS= -c -Wall -lm

all: rsa

rsa: RSA.o 
	$(CC) RSA.o -o rsa.exec
RSA.o: RSA.cpp
	$(CC) $(CFLAGS) RSA.cpp
test:
	exec ./rsa.exec < input
testar:
	python testador.py > ent
	python RSA.py < ent
	./rsa.exec < ent 2> rsa.exec.log 
clean:
	rm -rf *.o rsa.exec
