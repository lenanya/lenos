#include "common.h"
#include "vga.h"
#include "std/string.h"

static int __vga_current_x = 0;
static int __vga_current_y = 0;

void vga_clear_screen(uchar colour) {
  __vga_current_x = 0;
  __vga_current_y = 0;
  vga_wait_for_vblank();
  vga_set_cursor_position(0, 0);
  volatile VGA_Slot* vga_mem = (VGA_Slot*)VGA_MEM;
  for (int y = 0; y < VGA_ROWS; y++) {
    for (int x = 0; x < VGA_COLS; x++) {
      vga_mem[y*VGA_COLS+x] = (VGA_Slot){' ', colour};
    }
  }
}

void vga_disable_cursor(void) {
  outb(VGA_INDEX, VGA_CURSOR_START);
  outb(VGA_DATA, 0x20);
}

void vga_enable_cursor(void) {
  outb(VGA_INDEX, VGA_CURSOR_START);
  outb(VGA_DATA, 0x0);
}

void vga_set_cursor_position(int x, int y) {
  ushort pos = y * VGA_COLS + x;

  outb(VGA_INDEX, VGA_CURSOR_H);
  outb(VGA_DATA, (uchar)(pos >> 8) & 0xff);

  outb(VGA_INDEX, VGA_CURSOR_L);
  outb(VGA_DATA, (uchar)(pos & 0xff));
}

void vga_wait_for_vblank(void) {
  // wait for any current vblank to end
  while (inb(VGA_STATUS) & 0x08);
  // wait for the next one 
  while (!inb(VGA_STATUS) & 0x08);
}

void vga_putc_colour(uchar c, uchar colour) {
  vga_wait_for_vblank();
  VGA_Slot* vga_mem = (VGA_Slot*)VGA_MEM;
  if (c == '\n') {
    __vga_current_y++;
    __vga_current_x = 0;
    vga_set_cursor_position(__vga_current_x, __vga_current_y);
    return;
  }
  vga_mem[__vga_current_y * VGA_COLS + __vga_current_x] = (VGA_Slot){c, colour};
  
  __vga_current_x++;
  if (__vga_current_x >= VGA_COLS) {
    __vga_current_y++;
    __vga_current_x = 0;
  }
  vga_set_cursor_position(__vga_current_x, __vga_current_y);
}

void vga_delc(void) {
  VGA_Slot* vga_mem = (VGA_Slot*)VGA_MEM;
  __vga_current_x--;
  if (__vga_current_x <= 0) {
    __vga_current_x = 0;
    __vga_current_y--;
    if (__vga_current_y < 0) {
      __vga_current_y = 0;
    }
  }
  vga_set_cursor_position(__vga_current_x, __vga_current_y);
  vga_mem[__vga_current_y * VGA_COLS + __vga_current_x].c = ' ';
}

void vga_puts_colour(char* s, uchar colour) {
  int l = strlen(s);
  for (int i = 0; i < l; ++i) {
    vga_putc_colour(s[i], colour);
  }
}