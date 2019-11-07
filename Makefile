all: rsa

rsa: rsa_test.o rsa.o
		gcc -o rsa rsa_test.o rsa.o

rsa_test.o: rsa_test.c
		gcc -c rsa_test.c

rsa.o: rsa.c
		gcc -c rsa.c

clean:
		del *.o