all: memgrind

memgrind: memgrind.o mymalloc.o
	gcc -Wall -std=c99 -g -o memgrind memgrind.o mymalloc.o

memgrind.o: memgrind.c mymalloc.h
	gcc -Wall -std=c99 -g -c memgrind.c

mymalloc.o: mymalloc.c mymalloc.h
	gcc -Wall -std=c99 -g -c mymalloc.c