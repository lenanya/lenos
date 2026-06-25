#ifndef STRING_H
#define STRING_H

#include "../common.h"

/*
TODO: use this instead of null terminated ones

typedef struct String {
  u64 length;
  char* data;
};
*/

u32 strlen(char* s);
char* strdup(char* s);
bool strcmp(char* a, char* b);
bool strncmp(char* a, char* b, u32 n);
char* itoa(i32 i);
void itoa_buffer(i32 n, char* buf);
char* u32_to_hex(u32 n, bool upper);
void u32_to_hex_buffer(u32 n, char* buf, bool upper);
char* utoa(u32 i);
void utoa_buffer(u32 n, char* buf);
u32 stou32(char* s);
bool is_digit(char c);
bool s_is_digits(char* s);

#endif // STRING_H