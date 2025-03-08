#ifndef __MY_MALLOC_H__
#define __MY_MALLOC_H__

#include <stddef.h>

// Function declarations
void* ff_malloc(size_t size);
void ff_free(void* ptr);
void* bf_malloc(size_t size);
void bf_free(void* ptr);
unsigned long get_data_segment_size();
unsigned long get_data_segment_free_space_size();

#endif // __MY_MALLOC_H__