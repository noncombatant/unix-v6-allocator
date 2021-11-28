#include <stdio.h>

#include "alloc.h"

int main() {
  size_t sizes[] = {100, 1000, 333};
  for (int i = 0; i < 3; i++) {
    size_t s = sizes[i];
    char* p = v6alloc(s);
    size_t* size = (size_t*)(p - sizeof(void*));
    printf("p: %p (%zu bytes allocated)\n", p, *size);
    v6free(p);
  }
}
