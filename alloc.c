/*
 * C library -- alloc/free
 */

#include <stddef.h>
#include <unistd.h>

#include "alloc.h"

typedef struct FreelistBlock {
  size_t size;
  struct FreelistBlock* next;
} FreelistBlock;

FreelistBlock freelist[] = {{
    0,
    -1,
}};
size_t slop = sizeof(void*);

void* v6alloc(size_t asize) {
  size_t size;
  FreelistBlock* next;
  FreelistBlock* current;

  if ((size = asize) == 0)
    return (0);
  size += 3 * sizeof(void*);
  size &= ~01;
  for (;;) {
    for (current = freelist; (next = current->next) != -1; current = next) {
      if (next->size >= size) {
        if (size + slop >= next->size) {
          current->next = next->next;
          return (&next->next);
        }
        current = current->next = next + size;
        current->size = next->size - size;
        current->next = next->next;
        next->size = size;
        return (&next->next);
      }
    }
    asize = size < 1024 ? 1024 : size;
    if ((current = sbrk(asize)) == -1) {
      return (-1);
    }
    current->size = asize;
    v6free(&current->next);
  }
}

void v6free(char* aptr) {
  FreelistBlock* ptr;
  FreelistBlock* current;
  FreelistBlock* next;

  ptr = aptr - sizeof(void*);
  current = freelist;
  while ((next = current->next) < ptr)
    current = next;
  if (ptr + ptr->size == next) {
    ptr->size = +next->size;
    ptr->next = next->next;
    next = ptr;
  } else
    ptr->next = next;
  if (current + current->size == ptr) {
    current->size = +ptr->size;
    current->next = ptr->next;
  } else
    current->next = ptr;
}
