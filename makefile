my_malloc.so : my_malloc.c my_malloc.h
	gcc -c -fPIC my_malloc.c -o my_malloc.o
	gcc my_malloc.o -shared -o my_malloc.so
	rm my_malloc.o
