// Issues (TO DO):
// better realloc with regard to now available block merging/dividing functions,
// thread safety, 
// portability of data allignment (generalize the word size),
// overflow test in my_calloc,
// other strategy than first fit might be better,
// return free end memory region back to the system,
// why newly allocated memory is zeroed?
// smaller block_info_t in order to save some space (24-byte is possible?),
// copy block in 8B chunks instead of using memset

#include <stdio.h>
#include <stdlib.h>  // size_t declared here
#include <unistd.h>  // sbrk declared here 
#include <string.h>  // memcpy, memset declared here

#include "my_malloc.h"
#include "my_malloc_private.h"

block_info_t first_block = NULL;
block_info_t last_block = NULL;

void *my_malloc(size_t size) 
{
  size = allign(size);
  block_info_t block = find_reusable_block(size);
  if(block) 
  {
    block->is_free = 0;
    if( block->size >= size + BLOCK_INFO_SIZE + 8 )
    {
      divide_block(block, size);
    }
    return (void *)(block + 1);
  }
  block = request_new_block(size);
  if(block)
  {
    return (void *)(block + 1);
  }
  return NULL;
}

block_info_t find_reusable_block(size_t size) 
{
  block_info_t current = first_block;
  while( current && (!current->is_free || current->size < size) )
  {
    current = current->next;
  }
  return current;
}

block_info_t request_new_block(size_t size)
{
  block_info_t new_block = sbrk(0);
  if( sbrk(size + BLOCK_INFO_SIZE) == (void *)-1 )
  {
    return NULL;
  }
  else
  {
    new_block->size = size;
    new_block->is_free = 0;
    new_block->ptr = (void *)(new_block + 1);
    new_block->next = NULL;
    new_block->prev = last_block;
    if( last_block )
    {
      last_block->next = new_block;
    }
    last_block = new_block;
    if( !first_block ) 
    {
      first_block = new_block;
    }
    return new_block;
  }
}

void divide_block(block_info_t block, size_t size)
{
  if(!block)
  {
    return;
  }
  block_info_t new_block = (block_info_t)((char *)block + size + BLOCK_INFO_SIZE);
  new_block->size = block->size - size - BLOCK_INFO_SIZE;
  new_block->is_free = 1;
  new_block->ptr = (void *)(new_block + 1);
  new_block->next = block->next;
  new_block->prev = block;
  block->size = size;
  block->next = new_block;
  new_block->next->prev = new_block;
  if( last_block == block )
  {
    last_block = new_block;
  }
}

void *my_calloc(size_t count, size_t size)
{
  void *ptr = my_malloc(count * size);
  if(ptr)
  {
    memset(ptr, 0, allign(count * size));
  }
  return ptr; 
}

void *my_realloc(void *ptr, size_t size)
{
  if(!ptr)
  {
    return my_malloc(size);
  }
  if(!is_valid_pointer(ptr))
  {
    return NULL;
  }
  block_info_t block = (block_info_t)ptr - 1;
  if( block->size >= size)
  {
    return ptr;
  }
  else
  {
    void *new_ptr = my_malloc(size);
    if(new_ptr) 
    {
      memcpy(new_ptr, ptr, block->size);
      my_free(ptr);
    }
    return new_ptr;
  }
}

void my_free(void *ptr)
{
  if(is_valid_pointer(ptr))
  {
    block_info_t block = (block_info_t)ptr - 1;
    block->is_free = 1;
    if(block->prev && block->prev->is_free)
    {
      block = merge_block(block->prev);
    }
    if(block->next && block->next->is_free)
    {
      merge_block(block);
    }
  }
}

int is_valid_pointer(void *ptr)
{
  if(!ptr)
  {
    return 0;
  }
  if(first_block && ptr >= (void *)first_block + 1 && ptr < sbrk(0))
  {
    block_info_t block = (block_info_t)ptr - 1;
    return ptr == block->ptr;
  }
  return 0;
}

block_info_t merge_block(block_info_t block)
{
  block_info_t next = block->next;
  block->size += BLOCK_INFO_SIZE + next->size;
  block->next = next->next;
  if(next->next)
  {
    next->next->prev = block;
  }
  if(last_block == next)
  {
    last_block = block;
  }
  return block;
}

size_t allign(size_t n)
{
  if(n > 0)
  {
    return ((n - 1 >> 3) << 3) + 8;
  }
  else
  {
    return 0;
  }
}

void print_block_list()
{
  printf("## First block: %d\n", first_block);
  block_info_t current = first_block;
  while(current)
  {
    printf("## A: %d, S: %d, F: %d, N: %d, P: %d\n", 
        current,
        current->size, 
        current->is_free, 
        current->next, 
        current->prev); 
    current = current->next;
  }
  printf("## Last block: %d\n", last_block);
}
