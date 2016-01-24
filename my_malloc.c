#include <stdlib.h>

// int main(int argc, char** argv)
// {
//  return 0;
// }

void *my_malloc(size_t size) 
{
  return malloc(size);
}

void *my_calloc(size_t count, size_t size)
{
  return calloc(count, size);
}

void *my_realloc(void *ptr, size_t size)
{
  return realloc(ptr, size);
}

void my_free(void *ptr)
{
  free(ptr);
}
