CFLAGS=-Wall -Wextra -Werror

all:server

server: database.o hash.o
	gcc $(CFLAGS) database.o hash.o -o server

database.o: macro.h hash.h hash.c database.c
	gcc $(CFLAGS) -c database.c

hash.o: macro.h hash.h hash.c
	gcc $(CFLAGS) -c hash.c

clean: 
	rm -rf *.o dbms server

rebuild:clean all