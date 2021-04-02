CC=g++
CFLAGS= -c -Wall -lm

all: rsa

rsa: RSA.o 
	$(CC) RSA.o -o rsa.exec
RSA.o: RSA.cpp
	$(CC) $(CFLAGS) RSA.cpp
test:
	exec ./rsa.exec
clean:
	rm -rf *.o rsa.exec < input
