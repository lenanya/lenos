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
  char(*getc)   (void);
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
  LFS_Table_Entry* (*get_file_entry)(char* filename);
} Std_File;

typedef struct Std_String {
  char* (*utoa)(u32 n);
  u32   (*strlen)(char* s);
  u32   (*stou32)(char* s);
  bool  (*is_digit)(char c);
  bool  (*s_is_digits)(char* s);
} Std_String;

typedef struct Std {
  Std_Io io;
  Std_Mem mem;
  Std_File file;
  Std_String str;
} Std;

void init_std(Std* std);

#endif // STD_H