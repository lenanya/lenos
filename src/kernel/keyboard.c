#include "keyboard.h"
#include "../common.h"


// is the key being pressed down
bool kb_is_make(u8 scancode) {
  return (scancode & 0x80) == 0;
}

// is the key being lifted
bool kb_is_release(u8 scancode) {
  return !kb_is_make(scancode);
}

// read a scancode from the keyboard
u8 kb_get_scancode(void) {
  // wait for scancode 
  while (!(inb(KB_CTRL) & 0x01));

  return inb(KB_DATA);
}