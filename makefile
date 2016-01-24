all: libmy_malloc.so test 
	
libmy_malloc.so : my_malloc.c my_malloc.h
	gcc -c -fPIC my_malloc.c -o my_malloc.o
	gcc my_malloc.o -shared -o libmy_malloc.so
	rm my_malloc.o

test : test.c libmy_malloc.so
	gcc test.c -L. -lmy_malloc -o test

run :
	@LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH} ./test
