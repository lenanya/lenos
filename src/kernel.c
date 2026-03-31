#include "common.h"
#include "vga.h"
#include "keyboard.h"

bool shift = false;

void kernel_main() {
  vga_clear_screen(VGA_CYAN_ON_GREY);
  vga_flip();
  for(;;) {
    uchar sc = kb_get_scancode();
    if (kb_is_make(sc)) {
      if (sc == 0x0e) vga_delc();
      else {
        uchar ascii = kb_gb_map[sc];
        if (ascii) vga_putc_colour(ascii, VGA_CYAN_ON_GREY);
      }
    }
    vga_flip();
  }
}