#ifndef FILE_H
#define FILE_H

#include "../common.h"

typedef struct File_Buffer {
  char* items;
  uint size;
  uint capacity;
  bool exists;
} __attribute__((packed)) File_Buffer;

typedef enum {
  FILE,
} File_Type;

void read_entire_file(File_Buffer*fb, char* filename);
void file_buffer_free(File_Buffer* fb);

#endif // FILE_H