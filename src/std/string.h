#ifndef STRING_H
#define STRING_H

#include "../common.h"

typedef struct {
  char* items;
  uint size;
  uint capacity;
} __attribute__((packed)) String_Buffer;

int strlen(char* s);
char* strdup(char* s);
bool strcmp(char* a, char* b);
bool strncmp(char* a, char* b, uint n);
char* itoa(int i);

#endif // STRING_H