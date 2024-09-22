#include "memory.h"

static bool is_power_of_two(uintptr_t x) {
  return (x & (x-1)) == 0;
}

static uintptr_t align_forward(uintptr_t ptr, size_t align) {
  uintptr_t p, a, modulo;

  assert(is_power_of_two(align));

  p = ptr;
  a = (uintptr_t)align;
  // Same as (p % a) but faster as 'a' is a power of two
  modulo = p & (a-1);

  if (modulo != 0) {
    // If 'p' address is not aligned, push the address to the
    // next value which is aligned
    p += a - modulo;
  }
  return p;
}

void ArenaAllocator_init(ArenaAllocator *a, void *backing_buffer, usize backing_buffer_length) {
  a->buf = (u8 *)backing_buffer;
  a->buf_len = backing_buffer_length;
  a->curr_offset = 0;
  a->prev_offset = 0;
}

void *ArenaAllocator_allocAlign(ArenaAllocator *a, usize size, usize align) {
  // Align 'curr_offset' forward to the specified alignment
  uintptr_t curr_ptr = (uintptr_t)a->buf + (uintptr_t)a->curr_offset;
  uintptr_t offset = align_forward(curr_ptr, align);
  offset -= (uintptr_t)a->buf; // Change to relative offset

  // Check to see if the backing memory has space left
  if (offset+size <= a->buf_len) {
    void *ptr = &a->buf[offset];
    a->prev_offset = offset;
    a->curr_offset = offset+size;

    // Zero new memory by default
    memset(ptr, 0, size);
    return ptr;
  }
  // Return NULL if the arena is out of memory (or handle differently)
  return NULL;
}

void *ArenaAllocator_alloc(ArenaAllocator *a, usize size) {
  return ArenaAllocator_allocAlign(a, size, DEFAULT_ALIGNMENT);
}

void ArenaAllocator_free(ArenaAllocator *a, void *ptr) {
  (void)a;
  (void)ptr;
}

void *ArenaAllocator_resizeAlign(ArenaAllocator *a, void *old_memory, usize old_size, usize new_size, size_t align) {
  unsigned char *old_mem = (unsigned char *)old_memory;

  assert(is_power_of_two(align));

  if (old_mem == NULL || old_size == 0) {
    return ArenaAllocator_allocAlign(a, new_size, align);
  } else if (a->buf <= old_mem && old_mem < a->buf+a->buf_len) {
    if (a->buf+a->prev_offset == old_mem) {
      a->curr_offset = a->prev_offset + new_size;
      if (new_size > old_size) {
        // Zero the new memory by default
        memset(&a->buf[a->curr_offset], 0, new_size-old_size);
      }
      return old_memory;
    } else {
      void *new_memory = ArenaAllocator_allocAlign(a, new_size, align);
      size_t copy_size = old_size < new_size ? old_size : new_size;
      // Copy across old memory to the new memory
      memmove(new_memory, old_memory, copy_size);
      return new_memory;
    }

  } else {
    assert(0 && "Memory is out of bounds of the buffer in this arena");
    return NULL;
  }
}

void *ArenaAllocator_resize(ArenaAllocator *a, void *old_memory, usize old_size, usize new_size) {
  return ArenaAllocator_resizeAlign(a, old_memory, old_size, new_size, DEFAULT_ALIGNMENT);
}

void ArenaAllocator_freeAll(ArenaAllocator *a) {
  a->curr_offset = 0;
  a->prev_offset = 0;
}
