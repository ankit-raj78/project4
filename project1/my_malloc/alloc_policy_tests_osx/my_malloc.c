#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "my_malloc.h"
#include "stdint.h"
// Metadata structure for memory blocks
typedef struct block_meta {
    size_t size;            // Size of the block (excluding metadata)
    int free;              // 1 if block is free, 0 if allocated
    struct block_meta* next;// Pointer to the next block
    struct block_meta* prev;// Pointer to the previous block
} block_meta_t;

// Global variables
static block_meta_t* free_list_head = NULL;  // Head of our free list
static block_meta_t* heap_start = NULL;      // Start of our heap

// Helper Functions
static block_meta_t* get_block_ptr(void* ptr) {
    return (block_meta_t*)((char*)ptr - sizeof(block_meta_t));
}

static void* get_user_ptr(block_meta_t* block) {
    return (void*)((char*)block + sizeof(block_meta_t));
}

// Find a free block in the list using First Fit strategy
static block_meta_t* find_free_block_first_fit(size_t size) {
    block_meta_t* current = heap_start;
    while (current) {
        if (current->free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Find a free block in the list using Best Fit strategy
static block_meta_t* find_free_block_best_fit(size_t size) {
    block_meta_t* current = heap_start;
    block_meta_t* best_fit = NULL;
    size_t smallest_diff = SIZE_MAX;

    while (current) {
        if (current->free && current->size >= size) {
            size_t diff = current->size - size;
            if (diff < smallest_diff) {
                smallest_diff = diff;
                best_fit = current;
                if (diff == 0) break;  // Perfect fit found
            }
        }
        current = current->next;
    }
    return best_fit;
}

static block_meta_t* request_space(size_t size) {
    block_meta_t* block;
    block = sbrk(0);
    void* request = sbrk(size + sizeof(block_meta_t));
    if (request == (void*)-1) {
        return NULL;  // sbrk failed
    }

    block->size = size;
    block->free = 0;
    block->next = NULL;
    block->prev = NULL;
    
    // If this is the first allocation
    if (heap_start == NULL) {
        heap_start = block;
    } else {
        // Find the last block in the list
        block_meta_t* last = heap_start;
        while (last->next) {
            last = last->next;
        }
        // Link the new block
        last->next = block;
        block->prev = last;
    }
    
    return block;
}

// Add a block to the free list
static void add_to_free_list(block_meta_t* block) {
    if (!block) return;
    block->free = 1;
    
    // If free_list_head is NULL or if block should be inserted before free_list_head
    if (!free_list_head || block < free_list_head) {
        block->next = free_list_head;
        if (free_list_head) {
            free_list_head->prev = block;
        }
        free_list_head = block;
    } else {
        block_meta_t* current = free_list_head;
        // Find the correct position to insert the block
        while (current->next && current->next < block) {
            current = current->next;
        }
        block->next = current->next;
        block->prev = current;
        if (current->next) {
            current->next->prev = block;
        }
        current->next = block;
    }
}

// Merge adjacent free blocks
static void coalesce(block_meta_t* block) {
    if (!block) return;
    
    // Try to merge with next block
    if (block->next && block->next->free) {
        block->size += block->next->size + sizeof(block_meta_t);
        block->next = block->next->next;
        if (block->next) {
            block->next->prev = block;
        }
    }
    
    // Try to merge with previous block
    if (block->prev && block->prev->free) {
        block->prev->size += block->size + sizeof(block_meta_t);
        block->prev->next = block->next;
        if (block->next) {
            block->next->prev = block->prev;
        }
        block = block->prev;  // Move block pointer to the previous block
    }
}

// Split block if it's too large
static void split_block(block_meta_t* block, size_t size) {
    // Check if we can split (need enough space for a new block metadata and some data)
    if (block->size < size + sizeof(block_meta_t) + 8) {
        return;  // Not enough space to split
    }
    
    // Calculate the size of the new free block
    size_t remaining_size = block->size - size - sizeof(block_meta_t);
    
    // Create a new block from the remaining space
    block_meta_t* new_block = (block_meta_t*)((char*)get_user_ptr(block) + size);
    new_block->size = remaining_size;
    new_block->free = 1;
    new_block->next = block->next;
    new_block->prev = block;
    
    // Update the original block
    block->size = size;
    block->next = new_block;
    
    // Update the next block's prev pointer if it exists
    if (new_block->next) {
        new_block->next->prev = new_block;
    }
}

// First Fit Implementation
void* ff_malloc(size_t size) {
    if (size <= 0) return NULL;

    block_meta_t* block = find_free_block_first_fit(size);
    
    // If no suitable block found, request new space
    if (!block) {
        block = request_space(size);
        if (!block) return NULL;
    } else {
        // Remove block from free list since we're going to use it
        block->free = 0;
        // Split block if it's too large
        split_block(block, size);
    }
    
    return get_user_ptr(block);
}

void ff_free(void* ptr) {
    if (!ptr) return;
    
    // Get the block metadata
    block_meta_t* block = get_block_ptr(ptr);
    
    // Mark the block as free
    block->free = 1;
    
    // Add to free list and coalesce with adjacent blocks
    add_to_free_list(block);
    coalesce(block);
}

// Best Fit Implementation
void* bf_malloc(size_t size) {
    if (size <= 0) return NULL;

    block_meta_t* block = find_free_block_best_fit(size);
    
    // If no suitable block found, request new space
    if (!block) {
        block = request_space(size);
        if (!block) return NULL;
    } else {
        // Remove block from free list since we're going to use it
        block->free = 0;
        // Split block if it's too large
        split_block(block, size);
    }
    
    return get_user_ptr(block);
}

void bf_free(void* ptr) {
    if (!ptr) return;
    
    // Get the block metadata
    block_meta_t* block = get_block_ptr(ptr);
    
    // Mark the block as free
    block->free = 1;
    
    // Add to free list and coalesce with adjacent blocks
    add_to_free_list(block);
    coalesce(block);
}

// Performance measurement functions
unsigned long get_data_segment_size() {
    if (!heap_start) return 0;
    
    // Find the last block
    block_meta_t* last = heap_start;
    while (last->next) {
        last = last->next;
    }
    
    // Calculate total size from heap_start to end of last block
    return ((char*)last + sizeof(block_meta_t) + last->size) - (char*)heap_start;
}

unsigned long get_data_segment_free_space_size() {
    unsigned long free_space = 0;
    block_meta_t* current = heap_start;
    
    // Sum up all free blocks (including metadata)
    while (current) {
        if (current->free) {
            free_space += current->size + sizeof(block_meta_t);
        }
        current = current->next;
    }
    
    return free_space;
}
