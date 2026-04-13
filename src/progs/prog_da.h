#ifndef PROG_DA_H
#define PROG_DA_H

#define DA_INITIAL_SIZE 32

#define da_append(da, x) do {                                     \
  (da)->size++;                                                   \
  if ((da)->capacity == 0) {                                      \
    (da)->capacity = DA_INITIAL_SIZE;                             \
    (da)->items = std.mem.malloc((da)->capacity);                         \
  }                                                               \
  if ((da)->size > (da)->capacity) {                              \
    (da)->capacity = (da)->capacity * 2;                          \
    (da)->items = std.mem.realloc((da)->items, (da)->capacity);           \
  }                                                               \
  (da)->items[(da)->size-1] = x;                                  \
} while(0);

#endif // PROG_DA_H