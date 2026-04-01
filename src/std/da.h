#ifndef DA_H
#define DA_H
#include "mem.h"

#define DA_INITIAL_SIZE 32

#define da_append(da, x) do {                                     \
  if ((da)->capacity == 0) {                                      \
    (da)->capacity = DA_INITIAL_SIZE;                             \
    (da)->items = malloc((da)->capacity);                         \
  }                                                               \
  if ((da)->size > (da)->capacity) {                              \
    (da)->capacity = (da)->capacity * 2;                          \
    (da)->items = realloc((da)->items, (da)->capacity);           \
  }                                                               \
  (da)->items[(da)->size++] = x;                                  \
} while(0);

#define da_free(da) free((da)->items)

#endif // DA_H