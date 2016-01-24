#include <stdio.h>

#include "my_malloc.h"

int main(int argc, char** argv)
{
  int *iptr = (int*)my_malloc(sizeof(int));
  *iptr = 15;
  printf("Works fine, see: %d\n", *iptr);
  my_free(iptr);
  return 0;
}
