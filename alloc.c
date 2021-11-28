/*
 * C library -- alloc/free
 */

#include <stddef.h>
#include <unistd.h>

#include "alloc.h"

#define logical char*

struct fb {
  size_t size;
  struct fb* next;
};

struct fb freelist[] = {{
    0,
    -1,
}};
size_t slop = sizeof(void*);

void* v6alloc(size_t asize) {
  size_t size;
  struct fb* np;
  struct fb* cp;

  if ((size = asize) == 0)
    return (0);
  size += 3 * sizeof(void*);
  size &= ~01;
  for (;;) {
    for (cp = freelist; (np = cp->next) != -1; cp = np) {
      if (np->size >= size) {
        if (size + slop >= np->size) {
          cp->next = np->next;
          return (&np->next);
        }
        cp = cp->next = np + size;
        cp->size = np->size - size;
        cp->next = np->next;
        np->size = size;
        return (&np->next);
      }
    }
    asize = size < 1024 ? 1024 : size;
    if ((cp = sbrk(asize)) == -1) {
      return (-1);
    }
    cp->size = asize;
    v6free(&cp->next);
  }
}

void v6free(char* aptr) {
  struct fb* ptr;
  struct fb* cp;
  struct fb* np;

  ptr = aptr - sizeof(void*);
  cp = freelist;
  while ((np = cp->next) < ptr)
    cp = np;
  if (ptr + ptr->size == np) {
    ptr->size = +np->size;
    ptr->next = np->next;
    np = ptr;
  } else
    ptr->next = np;
  if (cp + cp->size == ptr) {
    cp->size = +ptr->size;
    cp->next = ptr->next;
  } else
    cp->next = ptr;
}
