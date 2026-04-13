#ifndef STD_H
#define STD_H
#include "common.h"

typedef struct Args {
  char** items;
  u32 size;
  u32 capacity;
} Args;

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

typedef struct Std_File {
  void (*read_entire_file)(File_Buffer* fb, char* filename);
  void (*file_buffer_free)(File_Buffer* fb);
  void (*write_entire_file)(File_Buffer* fb, char* filename);
  void (*delete_file)(char* filename);
} Std_File;

typedef struct Std {
  Std_Io io;
  Std_Mem mem;
  Std_File file;
} Std;

void init_std(Std* std);

#endif // STD_H