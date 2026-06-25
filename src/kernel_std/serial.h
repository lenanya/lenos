#ifndef SERIAL_H
#define SERIAL_H
#include "../common.h"
#include "../kernel/com1.h"

// so the API makes sense
#define serial_init() com1_init()
#define serial_putc(c) com1_putc(c)

void serial_print(char* s);
void serial_println(char* s);
void serial_printf(char* fmt, ...);

#endif // SERIAL_H