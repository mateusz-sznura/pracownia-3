// Issues: splitting blocks, merging adjacent free blocks,
// thread safety, aligning, reusing redundant blocks from struct,
// overflow test in my_calloc, checking for valid pointer in my_free,
// other strategy than first fit

#include <stdlib.h>  // size_t declared here
#include <unistd.h>  // sbrk declared here 
#include <string.h>  // memcpy, memset declared here

#include "my_malloc.h"
#include "my_malloc_private.h"

block_info_t first_block = NULL;
block_info_t last_block = NULL;

void *my_malloc(size_t size) 
{
  block_info_t block = find_reusable_block(size);
  if(block) 
  {
    // TO DO: Create new block from redundant memory
    block->is_free = 0;
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
  if( sbrk(allign(size) + BLOCK_INFO_SIZE) == (void *)-1 )
  {
    return NULL;
  }
  else
  {
    new_block->size = size;
    new_block->is_free = 0;
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
  if( !block )
  {
    return;
  }
  block_info_t new_block = (block_info_t)((char *)block + size + BLOCK_INFO_SIZE);
  new_block->size = block->size - size - BLOCK_INFO_SIZE;
  new_block->is_free = 1;
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
    memset(ptr, 0, count * size);
  }
  return ptr; 
}

void *my_realloc(void *ptr, size_t size)
{
  if( !ptr )
  {
    return my_malloc(size);
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
      block->is_free = 1;
    }
    return new_ptr;
  }
}

void my_free(void *ptr)
{
  if(ptr)
  {
    // TO DO: Check if ptr is valid pointer to beginning of occupied block
    // TO DO: Merge adjacent free blocks
    block_info_t block = (block_info_t)ptr - 1;
    block->is_free = 1;
  }
}

size_t allign(size_t n)
{
  // TO DO: Make it work for different word sizes
  if( n > 0 )
  {
    return ((n - 1 >> 3) << 3) + 8;
  }
  else
  {
    return 0;
  }
}
