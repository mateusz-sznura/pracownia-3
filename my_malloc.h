#ifndef _MY_MALLOC_H_
#define _MY_MALLOC_H_

void *my_malloc(size_t size);
void *my_calloc(size_t count, size_t size);
void *my_realloc(void *ptr, size_t size);
void my_free(void *ptr);

#endif
