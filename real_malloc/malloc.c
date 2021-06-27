////////////////////////////////////////////////////////////////////////////////
/*                 (๑＞◡＜๑)  Malloc Challenge!!  (◍＞◡＜◍)                   */
////////////////////////////////////////////////////////////////////////////////

//
// Welcome to Malloc Challenge!! Your job is to invent a smart malloc algorithm.
//
// Rules:
//
// 1. Your job is to implement my_malloc(), my_free() and my_initialize().
//   *  my_initialize() is called only once at the beginning of each challenge.
//      You can initialize the memory allocator.
//   *  my_malloc(size) is called every time an object is allocated. In this
//      challenge, |size| is guaranteed to be a multiple of 8 bytes and meets
//      8 <= size <= 4000.
//   * my_free(ptr) is called every time an object is freed.
//   * Additionally, my_finalize() is called only once at the end of each
//   challenge,
//     so you can use this function for doing some clean ups if you want.
// 2. The only library functions you can use in my_malloc() and my_free() are
//    mmap_from_system() and munmap_to_system().
//   *  mmap_from_system(size) allocates |size| bytes from the system. |size|
//      needs to be a multiple of 4096 bytes. mmap_from_system(size) is a
//      system call and heavy. You are expected to minimize the call of
//      mmap_from_system(size) by reusing the returned
//      memory region as much as possible.
//   *  munmap_to_system(ptr, size) frees the memory region [ptr, ptr + size)
//      to the system. |ptr| and |size| need to be a multiple of 4096 bytes.
//      You are expected to free memory regions that are unused.
//   *  You are NOT allowed to use any other library functions at all, including
//      the default malloc() / free(), std:: libraries etc. This is because you
//      are implementing malloc itself -- if you use something that may use
//      malloc internally, it will result in an infinite recurion.
// 3. simple_malloc(), simple_free() and simple_initialize() in simple_malloc.c
//    are an example of straightforward implementation.
//    Your job is to invent a smarter malloc algorithm than the simple malloc.
// 4. There are five challenges (Challenge 1, 2, 3, 4 and 5). Each challenge
//    allocates and frees many objects with different patterns. Your malloc
//    is evaluated by two criteria.
//   *  [Speed] How faster your malloc finishes the challange compared to
//      the simple malloc.
//   *  [Memory utilization] How much your malloc is memory efficient.
//      This is defined as (S1 / S2), where S1 is the total size of objects
//      allocated at the end of the challange and S2 is the total size of
//      mmap_from_system()ed regions at the end of the challenge. You can
//      improve the memory utilization by decreasing memory fragmentation and
//      reclaiming unused memory regions to the system with munmap_to_system().
// 5. This program works on Linux and Mac but not on Windows. If you don't have
//    Linux or Mac, you can use Google Cloud Shell (See
//    https://docs.google.com/document/d/1TNu8OfoQmiQKy9i2jPeGk1DOOzSVfbt4RoP_wcXgQSs/edit#).
// 6. You need to specify an '-lm' option to compile this program.
//   *  gcc malloc_challenge.c -lm
//   *  clang malloc_challenge.c -lm
//
// Enjoy! :D
//

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *mmap_from_system(size_t size);
void munmap_to_system(void *ptr, size_t size);

// Each object or free slot has metadata just prior to it:
//
// ... | m | object | m | free slot | m | free slot | m | object | ...
//
// where |m| indicates metadata. The metadata is needed for two purposes:
//
// 1) For an allocated object:
//   *  |size| indicates the size of the object. |size| does not include
//      the size of the metadata.
//   *  |next| is unused and set to NULL.
// 2) For a free slot:
//   *  |size| indicates the size of the free slot. |size| does not include
//      the size of the metadata.
//   *  The free slots are linked with a singly linked list (we call this a
//      free list). |next| points to the next free slot.
typedef struct metadata_t {
  size_t size;
  struct metadata_t *next;
  struct metadata_t *prev;
} metadata_t;

metadata_t *heap;

#define BUFFER_SIZE 4096

// my_initialize() is called only once at the beginning of each challenge.
void my_initialize() {
  // Implement here!
  heap = (metadata_t *)mmap_from_system(BUFFER_SIZE);
  heap->size = BUFFER_SIZE - sizeof(metadata_t);
  heap->next = NULL;
  heap->prev = NULL;
}

// Add a free slot to the beginning of the free list.
void add_to_free_list(metadata_t *metadata) {
  assert(!metadata->next);
  assert(!metadata->prev);
  
  // 繋げられる空き領域を探して、あったら繋げる
  metadata_t *comparison_prev = NULL;
  metadata_t *comparison = heap;
  while (comparison != NULL) {
    if ((metadata_t *)((char *)metadata + metadata->size + sizeof(metadata_t)) == comparison) {
      if (comparison_prev != NULL) comparison_prev->next = metadata;
      metadata->size = metadata->size + comparison->size + sizeof(metadata_t);
      metadata->next = comparison->next;
      metadata->prev = comparison_prev;

      // 繋げた空き領域が BUFFER_SIZE より大きくなったら解放
      if (metadata->size > BUFFER_SIZE) {
        void *ptr = (void *)((char *)metadata + sizeof(metadata_t) + BUFFER_SIZE);
        munmap_to_system(ptr, BUFFER_SIZE);
        metadata->size -= BUFFER_SIZE;
      }

      return;
    }

    if ((metadata_t *)((char *)comparison + comparison->size + sizeof(metadata_t)) == metadata) {
      comparison->size = comparison->size + metadata->size + sizeof(metadata_t);

      // 繋げた空き領域が BUFFER_SIZE より大きくなったら解放
      if (comparison->size > BUFFER_SIZE) {
        void *ptr = (void *)((char *)comparison + sizeof(metadata_t) + BUFFER_SIZE);
        munmap_to_system(ptr, BUFFER_SIZE);
        comparison->size -= BUFFER_SIZE;
      }

      return;
    }

    comparison_prev = comparison;
    comparison = comparison->next;
  }

  // 繋げられる空き領域が無かったら
  if (heap != NULL) {
    metadata->next = heap;
    heap = metadata;
    heap->next->prev = heap;
  } else {
    heap = metadata;
  }
}

// Remove a free slot from the free list.
void remove_from_free_list(metadata_t *metadata) {
  metadata_t *prev = metadata->prev;
  metadata_t *next = metadata->next;

  if (prev != NULL && next != NULL) {
    prev->next = next;
  } else if (prev == NULL && next != NULL) {
    heap = next;
  } else if (prev != NULL && next == NULL) {
    prev->next = NULL;
  } else {
    heap = NULL;
  }

  metadata->next = NULL;
  metadata->prev = NULL;
}

// my_malloc() is called every time an object is allocated. |size| is guaranteed
// to be a multiple of 8 bytes and meets 8 <= |size| <= 4000. You are not
// allowed to use any library functions other than mmap_from_system /
// munmap_to_system.
void *my_malloc(size_t size) {
  // Implement here!
  metadata_t *metadata = NULL;
  metadata_t *now = heap;

  // First-fit: Find the first free slot the object fits.
  /* while (now != NULL) {
    if (now->size >= size) {
      metadata = now;
      break;
    }
    now = now->next;
  } */

  // Best-fit: Find the best free slot the object fits.
  while (now != NULL) {
    if ((metadata == NULL || metadata->size > now->size) && now->size >= size) {
      metadata = now;
    }
    now = now->next;
  }

  // Worst-fit: Find the worst free slot the object fits.
  /* while (now != NULL) {
    if ((metadata == NULL || metadata->size < now->size) && now->size >= size) {
      metadata = now;
    }
    now = now->next;
  } */


  if (metadata == NULL) {
    // There was no free slot available. We need to request a new memory region
    // from the system by calling mmap_from_system().
    //
    //     | metadata | free slot |
    //     ^
    //     metadata
    //     <---------------------->
    //            buffer_size
    metadata = (metadata_t *)mmap_from_system(BUFFER_SIZE);
    metadata->size = BUFFER_SIZE - sizeof(metadata_t);
    metadata->next = NULL;
    metadata->prev = NULL;
    // Add the memory region to the free list.
    add_to_free_list(metadata);
    // Now, try my_malloc() again. This should succeed.
    return my_malloc(size);
  }

  // |ptr| is the beginning of the allocated object.
  //
  // ... | metadata | object | ...
  //     ^          ^
  //     metadata   ptr
  void *ptr = metadata + 1;
  size_t remaining_size = metadata->size - size;

  // Remove the free slot from the free list.
  remove_from_free_list(metadata);

  if (remaining_size > sizeof(metadata_t)) {
    // Create a new metadata for the remaining free slot.
    //
    // ... | metadata | object | metadata | free slot | ...
    //     ^          ^        ^
    //     metadata   ptr      new_metadata
    //                 <------><---------------------->
    //                   size       remaining size
    metadata_t *new_metadata = (metadata_t *)((char *)ptr + size);
    new_metadata->size = remaining_size - sizeof(metadata_t);
    new_metadata->next = NULL;
    new_metadata->prev = NULL;
    // Add the remaining free slot to the free list.
    add_to_free_list(new_metadata);

    metadata->size = size;
  }

  return ptr;
}

// my_free() is called every time an object is freed.  You are not allowed to
// use any library functions other than mmap_from_system / munmap_to_system.
void my_free(void *ptr) {
  // Implement here!
  // Look up the metadata. The metadata is placed just prior to the object.
  //
  // ... | metadata | object | ...
  //     ^          ^
  //     metadata   ptr
  metadata_t *metadata = (metadata_t *)ptr - 1;
  // Add the free slot to the free list.
  add_to_free_list(metadata);
}

void my_finalize() {
  // Implement here!
}

void test() {
  // Implement here!
  assert(1 == 1); /* 1 is 1. That's always true! (You can remove this.) */
}
