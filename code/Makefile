CFLAGS=-Wall -Wextra -Werror -g
ENTRY=./structures/
STRUCTURES_C = *.c
STRUCTURES_O = *.o

all: server

server: structures.a
	gcc $(CFLAGS) database.c structures.a -o server
	
structures.a:
	gcc $(CFLAGS) -c $(ENTRY)$(STRUCTURES_C) 
	ar rcs structures.a $(STRUCTURES_O)
	rm -rf *.o

clean: 
	rm -rf *.o server structures.a

rebuild: clean all

default: rebuild