all: memgrind

memgrind: memgrind.o mymalloc.o
	gcc -Wall -std=c99 -g -o memgrind memgrind.o mymalloc.o

memgrind.o: memgrind.c mymalloc.h
	gcc -Wall -std=c99 -g -c memgrind.c

mymalloc.o: mymalloc.c mymalloc.h
	gcc -Wall -std=c99 -g -c mymalloc.c

test1: test1.o mymalloc.o
	gcc -Wall -std=c99 -g -o test1 test1.o mymalloc.o

test1.o: test1.c mymalloc.h
	gcc -Wall -std=c99 -g -c test1.c

test2: test2.o mymalloc.o
	gcc -Wall -std=c99 -g -o test2 test2.o mymalloc.o

test2.o: test2.c mymalloc.h
	gcc -Wall -std=c99 -g -c test2.c

test3: test3.o mymalloc.o
	gcc -Wall -std=c99 -g -o test3 test3.o mymalloc.o

test3.o: test3.c mymalloc.h
	gcc -Wall -std=c99 -g -c test3.c

test4: test4.o mymalloc.o
	gcc -Wall -std=c99 -g -o test4 test4.o mymalloc.o

test4.o: test4.c mymalloc.h
	gcc -Wall -std=c99 -g -c test4.c

test5: test5.o mymalloc.o
	gcc -Wall -std=c99 -g -o test5 test5.o mymalloc.o

test5.o: test5.c mymalloc.h
	gcc -Wall -std=c99 -g -c test5.c

memtest: memtest.o mymalloc.o
	gcc -Wall -std=c99 -g -o memtest memtest.o mymalloc.o

memtest.o: memtest.c mymalloc.h
	gcc -Wall -std=c99 -g -c memtest.c

memtest-leak: memtest.c mymalloc.c mymalloc.h
	gcc -Wall -std=c99 -g -DLEAK -o memtest-leak memtest.c mymalloc.c

memtest-real: memtest.c
	gcc -Wall -std=c99 -g -DREALMALLOC -o memtest-real memtest.c