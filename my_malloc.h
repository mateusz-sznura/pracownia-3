#ifndef _MY_MALLOC_H_
#define _MY_MALLOC_H_

void *malloc(size_t size);
void *calloc(size_t count, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

#endif
