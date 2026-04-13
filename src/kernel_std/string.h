#ifndef STRING_H
#define STRING_H

#include "../common.h"

typedef struct {
  char* items;
  u32 size;
  u32 capacity;
} String_Buffer;

int strlen(char* s);
char* strdup(char* s);
bool strcmp(char* a, char* b);
bool strncmp(char* a, char* b, u32 n);
char* itoa(i32 i);
char* u32_to_hex(u32 n);
char* utoa(u32 i);

#endif // STRING_H