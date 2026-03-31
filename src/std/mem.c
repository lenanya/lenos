#include "mem.h"

void memncpy(void* src, void* dest, int n) {
  for (int i = 0; i < n; ++i) {
    ((char*)dest)[i] = ((char*)src)[i];
  }
}