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
	echo py: > log
	python RSA.py < ent 2>> log
	echo cpp: >> log
	./rsa.exec < ent 2>> log 
clean:
	rm -rf *.o rsa.exec
