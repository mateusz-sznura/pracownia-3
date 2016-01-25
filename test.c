#include <stdio.h>  // printf declared here
#include <string.h>  // strcpy declared here
#include <stdlib.h>  // malloc

#include "my_malloc.h"
#include "my_malloc_private.h"

void print_sizes()
{
  printf("-------- Sizes of types\n");
  printf("sizeof(int): %d\n", sizeof(int));
  printf("sizeof(long): %d\n", sizeof(long));
  printf("sizeof(void*): %d\n", sizeof(void*));
  printf("sizeof(size_t): %d\n", sizeof(size_t));
  printf("sizeof(struct block_info_t): %d\n", sizeof(struct block_info_t));
}

void test_case_1()
{
  // Alloc, free, alloc again - check if allocated the same space
  printf("-------- Test case 1\n");
  int *ptr = (int *)my_malloc(sizeof(int));
  printf("Address: %d, value (before assignment): %d\n", ptr, *ptr);
  *ptr = 100;
  printf("Address: %d, value (after assignment): %d\n", ptr, *ptr);
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

void test_case_3()
{
  // Allocate memory for string
  printf("-------- Test case 3\n");
  char *str = my_malloc(12 * sizeof(char));
  strcpy(str, "Ala ma kota");
  printf("Address: %d, string (allocated 12 bytes): %s\n", str, str);
}

void test_case_4()
{
  // Test calloc
  printf("-------- Test case 4\n");
  int *ptr_1 = my_calloc(1, sizeof(int));
  int *ptr_2 = my_malloc(sizeof(int));
  printf("Address: %d, value (calloc): %d\n", ptr_1, *ptr_1);
  printf("Address: %d, value (malloc): %d\n", ptr_2, *ptr_2);
}

void test_case_5()
{
  printf("-------- Test case 5\n");
  printf("-------- Data allignment\n");
  char *ptr_1 = my_malloc(11);
  int *ptr_2 = my_malloc(sizeof(int));
  printf("Address: %d (11 chars)\n", ptr_1);
  printf("Address: %d (int)\n", ptr_2);
}

void test_6()
{
  printf("-------- Test 6\n");
  printf("-------- Demonstrate block splitting (+ exhausting allocated space)\n");
  
  int *array = (int *)my_malloc(20 * sizeof(int));
  int *after = (int *)my_malloc(sizeof(int));
  *after = 100;
  for(int i = 0; i < 20; ++i)
  {
    array[i] = i;
  }
  printf("Address: %d, value: %d (array[0])\n", array, array[0]);
  printf("Address: %d, value: %d (after)\n", after, *after);

  printf("Exhausting allocated space does not overwrite next block? %d\n", ((block_info_t)after - 1)->size == sizeof(size_t));  

  my_free(array);

  long *num_1 = (long *)my_malloc(sizeof(long));
  long *num_2 = (long *)my_malloc(sizeof(long));
  *num_1 = 500;
  *num_2 = 600;
  printf("Address: %d, value: %d (num_1)\n", num_1, *num_1);
  printf("Address: %d, value: %d (num_2)\n", num_2, *num_2);

  print_block_list();
}

int main(int argc, char** argv)
{
  // int *p = malloc(0);
  // printf("%d, %d\n", p, *p);
  // print_sizes();
  // test_case_1();
  // test_case_2();
  // test_case_3();
  // test_case_4();
  // test_case_5();
  test_6();
  return 0;
}
