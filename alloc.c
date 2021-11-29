/*
 * C library -- alloc/free
 */

#define _DEFAULT_SOURCE
#include <stddef.h>
#include <unistd.h>

#include "alloc.h"

typedef struct FreelistBlock {
  size_t size;
  struct FreelistBlock* next;
} FreelistBlock;

#define END ((FreelistBlock*)-1)

static FreelistBlock freelist[] = {{
    0,
    END,
}};
size_t slop = sizeof(void*);

void* v6alloc(size_t asize) {
  if (asize == 0) {
    return 0;
  }
  size_t size = asize;
  size += 3 * sizeof(void*);
  size &= ~01U;

  for (;;) {
    FreelistBlock* next;
    FreelistBlock* current;
    for (current = freelist; (next = current->next) != END; current = next) {
      if (next->size >= size) {
        if (size + slop >= next->size) {
          current->next = next->next;
          return &next->next;
        }
        current = current->next = next + size;
        current->size = next->size - size;
        current->next = next->next;
        next->size = size;
        return &next->next;
      }
    }
    asize = size < 1024 ? 1024 : size;
    if ((current = sbrk(asize)) == END) {
      return NULL;
    }
    current->size = asize;
    v6free(&current->next);
  }
}

void v6free(void* aptr) {
  FreelistBlock* ptr = (FreelistBlock*)(((char*)aptr) - sizeof(void*));
  FreelistBlock* current = freelist;
  FreelistBlock* next;
  while ((next = current->next) < ptr) {
    current = next;
  }
  if (ptr + ptr->size == next) {
    ptr->size = +next->size;
    ptr->next = next->next;
    next = ptr;
  } else {
    ptr->next = next;
  }
  if (current + current->size == ptr) {
    current->size = +ptr->size;
    current->next = ptr->next;
  } else {
    current->next = ptr;
  }
}
