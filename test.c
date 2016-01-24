#include <stdio.h>

#include "my_malloc.h"
#include "my_malloc_private.h"

void print_sizes()
{
  printf("-------- Sizes of types\n");
  printf("sizeof(int): %d\n", sizeof(int));
  printf("sizeof(void*): %d\n", sizeof(void*));
  printf("sizeof(size_t): %d\n", sizeof(size_t));
  printf("sizeof(struct block_info_t): %d\n", sizeof(struct block_info_t));
}

void test_case_1()
{
  // Alloc, free, alloc again - check if allocated the same space
  printf("-------- Test case 1\n");
  int *ptr = (int *)my_malloc(sizeof(int));
  *ptr = 100;
  printf("Address: %d, value: %d\n", ptr, *ptr);
  my_free(ptr);
  int *ptr_2 = (int *)my_malloc(sizeof(int));
  printf("Same address? %d\n", ptr == ptr_2 );
  printf("Address: %d, value: %d\n", ptr_2, *ptr_2);
}

void test_case_2()
{
  // Multiple allocs - check if allocated different space
  printf("-------- Test case 2\n");
  int *ptr_1 = (int *)my_malloc(sizeof(int));
  int *ptr_2 = (int *)my_malloc(sizeof(int));
  int *ptr_3 = (int *)my_malloc(sizeof(int));
  *ptr_1 = 100;
  *ptr_2 = 200;
  *ptr_3 = 300;
  printf("Address: %d, value: %d\n", ptr_1, *ptr_1);
  printf("Address: %d, value: %d\n", ptr_2, *ptr_2);
  printf("Address: %d, value: %d\n", ptr_3, *ptr_3);
}

int main(int argc, char** argv)
{
  print_sizes();
  test_case_1();
  test_case_2();
  return 0;
}
