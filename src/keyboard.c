#include "keyboard.h"
#include "common.h"

bool kb_is_make(uchar scancode) {
  return (scancode & 0x80) == 0;
}

bool kb_is_release(uchar scancode) {
  return !kb_is_make(scancode);
}

uchar kb_get_scancode(void) {
  // wait for scancode 
  while (!(inb(KB_CTRL) & 0x01));

  return inb(KB_DATA);
}