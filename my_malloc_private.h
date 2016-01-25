#ifndef _MY_MALLOC_PRIVATE_H_
#define _MY_MALLOC_PRIVATE_H_

typedef struct block_info_t {
  size_t size;
  int is_free;
  char _padding[4];
  void *ptr;
  struct block_info_t *next;
  struct block_info_t *prev;
} *block_info_t;

#define BLOCK_INFO_SIZE sizeof(struct block_info_t)

block_info_t find_reusable_block(size_t size);
block_info_t request_new_block(size_t size);

void divide_block(block_info_t block, size_t size);
block_info_t merge_block(block_info_t block);

int is_valid_pointer(void *ptr);

size_t allign(size_t n);

#endif
