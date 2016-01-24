#ifndef _MY_MALLOC_PRIVATE_H_
#define _MY_MALLOC_PRIVATE_H_

typedef struct block_info_t {
  size_t size;
  int is_free;
  struct block_info_t *next;
  struct block_info_t *prev;
} *block_info_t;

#define BLOCK_INFO_SIZE sizeof(struct block_info_t)

block_info_t find_reusable_block(size_t size);
block_info_t request_new_block(size_t size);

#endif
