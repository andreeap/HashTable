CC = gcc
CFLAGS = -Wall -g

build: tema0

hash.o:	hash.c hash.h
	$(CC) -Wall -c -o hash.o hash.c

hashtable.o:	hashtable.c hashtable.h
	$(CC) -Wall -c -o hashtable.o hashtable.c

tema0: tema0.c hashtable.o hash.o 
	$(CC) $(CFLAGS) -o tema0 tema0.c hash.o hashtable.o 

clean:
	rm -f *.o *~ tema0
