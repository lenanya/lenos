#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "../common.h"

#define KB_DATA   0x60
#define KB_CTRL   0x64

// shamelessly copy pasted im not doing this manually
static u8 kb_gb_map[128] = {
    0,  0, '1', '2', '3', '4', '5', '6', '7', '8',
  '9', '0', '-', '=', 0,
  0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
   '\'', '`', 0, '#', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 
    0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-',
    0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, '\\', 0, 0 
};

static u8 kb_gb_shift_map[128] = {
    0,  0, '!', '"', 0, '$', '%', '^', '&', '*',	
  '(', ')', '_', '+', 0,
  0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
   '@', 0, 0, '~', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 
    0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-',
    0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, '|', 0, 0 
};

bool kb_is_make(u8 scancode);
u8 kb_get_scancode(void);
bool kb_is_release(u8 scancode);

#endif // KEYBOARD_H