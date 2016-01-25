#include <stdio.h> 
#include <string.h>  // strcpy declared here
#include <stdlib.h>  // malloc declared here

#include "my_malloc.h"
#include "my_malloc_private.h"

void demo_0()
{
  printf("-------- Demo 0\n");
  printf("-------- Type sizes\n");
  printf("%20s : %d\n", "int", sizeof(int));
  printf("%20s : %d\n", "long", sizeof(long));
  printf("%20s : %d\n", "void*", sizeof(void*));
  printf("%20s : %d\n", "size_t", sizeof(size_t));
  printf("%20s : %d\n", "struct block_info_t", sizeof(struct block_info_t));
}

void demo_1()
{
  printf("-------- Demo 1\n");
  printf("Alloc, free, alloc again - check if allocated the same space\n");
  int *ptr = (int *)my_malloc(sizeof(int));
  printf("Address: %d, value (before assignment): %d\n", ptr, *ptr);
  *ptr = 100;
  printf("Address: %d, value (after assignment): %d\n", ptr, *ptr);
  my_free(ptr);
  int *ptr_2 = (int *)my_malloc(sizeof(int));
  printf("Same address? %d\n", ptr == ptr_2 );
  printf("Address: %d, value: %d\n", ptr_2, *ptr_2);
}

void demo_2()
{
  printf("-------- Demo 2\n");
  printf("-------- Multiple allocs - check if allocated different space\n");
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

void demo_3()
{
  printf("-------- Demo 3\n");
  printf("-------- Allocate memory for string\n");
  char *str = my_malloc(12 * sizeof(char));
  strcpy(str, "Ala ma kota");
  printf("Address: %d, string (allocated 12 bytes): %s\n", str, str);
}

void demo_4()
{
  printf("-------- Demo 4\n");
  printf("-------- Demo calloc\n");
  int *ptr_1 = my_calloc(1, sizeof(int));
  int *ptr_2 = my_malloc(sizeof(int));
  printf("Address: %d, value (calloc): %d\n", ptr_1, *ptr_1);
  printf("Address: %d, value (malloc): %d\n", ptr_2, *ptr_2);
}

void demo_5()
{
  printf("-------- Demo 5\n");
  printf("-------- Data allignment\n");
  char *ptr_1 = my_malloc(11);
  int *ptr_2 = my_malloc(sizeof(int));
  printf("Address: %d (11 chars)\n", ptr_1);
  printf("Address: %d (int)\n", ptr_2);
}

void demo_6()
{
  printf("-------- Demo 6\n");
  printf("-------- Demonstrate block splitting (+ exhausting allocated space)\n");
  
  int *array = (int *)my_malloc(24 * sizeof(int));
  int *after = (int *)my_malloc(sizeof(int));
  *after = 100;
  for(int i = 0; i < 24; ++i)
  {
    array[i] = i;
  }
  printf("Address: %d, value: %d (array[0])\n", array, array[0]);
  printf("Address: %d, value: %d (after)\n", after, *after);

  printf("Exhausting allocated space does not overwrite next block? %d\n", 
      ((block_info_t)after - 1)->size == sizeof(size_t));  

  my_free(array);

  long *num_1 = (long *)my_malloc(sizeof(long));
  long *num_2 = (long *)my_malloc(sizeof(long));
  *num_1 = 500;
  *num_2 = 600;
  printf("Address: %d, value: %d (num_1)\n", num_1, *num_1);
  printf("Address: %d, value: %d (num_2)\n", num_2, *num_2);

  print_block_list();
}

void demo_7()
{
  printf("-------- Demo 7\n");
  printf("-------- Demonstrate block merging\n");
  int *ptr_1 = (int *)my_malloc(sizeof(int));
  int *ptr_2 = (int *)my_malloc(sizeof(int));
  int *ptr_3 = (int *)my_malloc(sizeof(int));
  int *ptr_4 = (int *)my_malloc(sizeof(int));
 
  my_free(ptr_1);
  my_free(ptr_3);
  printf("After freeing ptr_1 and ptr_3\n");
  print_block_list();

  my_free(ptr_4);
  printf("After freeing ptr_4\n");
  print_block_list();

  my_free(ptr_2);
  printf("After freeing ptr_2\n");
  print_block_list();
}

int main(int argc, char** argv)
{
  demo_0();
  // demo_1();
  // demo_2();
  // demo_3();
  // demo_4();
  // demo_5();
  // demo_6();
  demo_7();
  return 0;
}
