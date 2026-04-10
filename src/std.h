#ifndef STD_H
#define STD_H
#include "common.h"

typedef struct Std_Io {
  void(*print)  (char* s);
  void(*println)(char* s);
  void(*putc)   (char c);
  void(*printf) (char* fmt, ...);
} Std_Io;

typedef struct Std_Mem {
  void (*memncpy) (void* src, void* dest, u32 n);
  void*(*malloc)  (u32 size);
  void (*free)    (void* mem);
  void*(*realloc) (void* mem, u32 new_size);
  void*(*calloc)  (u32 size, char b);
  void*(*memdup)  (void* mem, u32 size);
} Std_Mem;

typedef struct Std {
  Std_Io io;
  Std_Mem mem;
} Std;

void init_std(Std* std);

#endif // STD_H