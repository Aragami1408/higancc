#ifndef HIGANCC_MEMORY_H
#define HIGANCC_MEMORY_H

// IN COURTESY OF GINGER BILL

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "common.h"

/*
 * ARENA ALLOCATOR
 */


#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2 * sizeof(void *))
#endif

typedef struct ArenaAllocator ArenaAllocator;

struct ArenaAllocator {
	u8 *buf;
	usize buf_len;
	usize prev_offset;
	usize curr_offset;
};

void ArenaAllocator_init(ArenaAllocator *a, void *backing_buffer, usize backing_buffer_length);
void *ArenaAllocator_allocAlign(ArenaAllocator *a, usize size, usize align);
void *ArenaAllocator_alloc(ArenaAllocator *a, usize size);
void *ArenaAllocator_resizeAlign(ArenaAllocator *a, void *old_memory, usize old_size, usize new_size, size_t align);
void *ArenaAllocator_resize(ArenaAllocator *a, void *old_memory, usize old_size, usize new_size);
void ArenaAllocator_freeAll(ArenaAllocator *a);

#endif
