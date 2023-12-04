CFLAGS=-Wall -Wextra -Werror

all:server

server: database.o hash.o stack.o array.o list.o tree.o queue.o hash.o double_list.o set.o
	gcc $(CFLAGS) database.o hash.o array.o tree.o stack.o list.o queue.o double_list.o set.o -o server

database.o: macro.h hash.h hash.c queue.c queue.h list.c list.h stack.h stack.c database.c
	gcc $(CFLAGS) -c database.c

hash.o: macro.h hash.h hash.c
	gcc $(CFLAGS) -c hash.c

stack.o: macro.h stack.h stack.c
	gcc $(CFLAGS) -c stack.c

list.o: macro.h list.h list.c
	gcc $(CFLAGS) -c list.c

double_list.o: macro.h double_list.h double_list.c
	gcc $(CFLAGS) -c double_list.c

set.o: macro.h set.h set.c
	gcc $(CFLAGS) -c set.c

queue.o: macro.h queue.h queue.c 
	gcc $(CFLAGS) -c queue.c

array.o: macro.h array.h array.c 
	gcc $(CFLAGS) -c array.c

three.o: macro.h tree.h tree.c 
	gcc $(CFLAGS) -c tree.c

clean: 
	rm -rf *.o server

rebuild:clean all