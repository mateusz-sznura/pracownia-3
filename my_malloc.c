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
  // first-fit strategy
  block_info_t current = first_block;
  while( current && (!current->is_free || current->size < size) )
  {
    current = current->next;
  }
  return current;
}

block_info_t request_new_block(size_t size)
{
  // TO DO: Align block size to 8 octets
  // Not thread safe!
  block_info_t new_block = sbrk(0);
  if( sbrk(size + BLOCK_INFO_SIZE) == (void *)-1 )
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
