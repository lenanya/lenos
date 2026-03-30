#include "common.h"
#include "vga.h"

void kernel_main() {
  vga_clear_screen(VGA_CYAN_ON_GREY);
  vga_disable_cursor();
  vga_puts_colour("Hello World!\n", VGA_CYAN_ON_GREY);
  vga_puts_colour("w dua lipa", VGA_CYAN_ON_GREY);
  for(;;);
}

