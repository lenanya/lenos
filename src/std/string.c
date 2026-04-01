#include "string.h"
#include "../common.h"
#include "mem.h"

int strlen(char* s) {
  int i = 0;
  while (s[i] != 0) ++i;
  return i;
}

void strdup(char* src, char* dest) {
  int len = strlen(src); 
  memncpy(src, dest, len);
}

bool strcmp(char* a, char* b) {
  int la, lb;
  la = strlen(a);
  lb = strlen(b);
  if (la != lb) return false;
  for (int i = 0; i < la; ++i) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

bool strncmp(char* a, char* b, uint n) {
  for (int i = 0; i < n; ++i) {
    if (a[i] != b[i]) return false;
  }
  return true;
}