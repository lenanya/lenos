#ifndef IO_H
#define IO_H

#include "../common.h"

void println(char* s);
void print(char* s);
void putc(char c);
void eprint(char* s);
void eprintln(char* s);
void printf(char* fmt, ...);
char getc();

#endif // IO_H