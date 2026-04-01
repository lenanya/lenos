#ifndef STRING_H
#define STRING_H

#include "../common.h"

typedef struct {
  char* items;
  uint size;
  uint capacity;
} __attribute__((packed)) String_Buffer;

int strlen(char* s);
void strdup(char* src, char* dest);
bool strcmp(char* a, char* b);
bool strncmp(char* a, char* b, uint n);

#endif // STRING_H