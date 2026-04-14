#ifndef STRING_H
#define STRING_H

#include "../common.h"

u32 strlen(char* s);
char* strdup(char* s);
bool strcmp(char* a, char* b);
bool strncmp(char* a, char* b, u32 n);
char* itoa(i32 i);
char* u32_to_hex(u32 n);
char* utoa(u32 i);
u32 stou32(char* s);
bool is_digit(char c);
bool s_is_digits(char* s);

#endif // STRING_H