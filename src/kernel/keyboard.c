#include "keyboard.h"
#include "../common.h"

bool kb_is_make(u8 scancode) {
  return (scancode & 0x80) == 0;
}

bool kb_is_release(u8 scancode) {
  return !kb_is_make(scancode);
}

u8 kb_get_scancode(void) {
  // wait for scancode 
  while (!(inb(KB_CTRL) & 0x01));

  return inb(KB_DATA);
}