#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "common.h"

#define KB_DATA   0x60
#define KB_STATUS 0x64

// GB Layout: Index is the scancode
static uchar kb_gb_map[128] = {
    0,  0, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
   '\'', '`', 0, '#', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 
    0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-',
    0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, '\\', 0, 0 /* 0x56 is backslash */
};

// Shifted GB Layout
static uchar kb_gb_shift_map[128] = {
    0,  0, '!', '"', 0, '$', '%', '^', '&', '*',	/* 156 is £ in extended ASCII */
  '(', ')', '_', '+', '\b',
  '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
   '@', 0, 0, '~', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 
    0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-',
    0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, '|', 0, 0 
};

bool kb_is_make(uchar scancode);
uchar kb_get_scancode(void);
bool kb_is_release(uchar scancode);

#endif // KEYBOARD_H