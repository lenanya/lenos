#include "common.h"
#include "vga.h"
#include "std/string.h"
#include "std/mem.h"

static int __vga_current_x = 0;
static int __vga_current_y = 0;

static VGA_Row* screen_buf = (VGA_Row*)(0xb9000);
static VGA_Row* vga_mem    = (VGA_Row*)VGA_MEM;

void vga_clear_screen(u8 colour) {
  __vga_current_x = 0;
  __vga_current_y = 0;
  vga_set_cursor_position(0, 0);
  for (int y = 0; y < VGA_ROWS; y++) {
    for (int x = 0; x < VGA_COLS; x++) {
      screen_buf[y][x] = (VGA_Slot){0, colour};
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
  u16 pos = y * VGA_COLS + x;

  outb(VGA_INDEX, VGA_CURSOR_H);
  outb(VGA_DATA, (u8)(pos >> 8) & 0xff);

  outb(VGA_INDEX, VGA_CURSOR_L);
  outb(VGA_DATA, (u8)(pos & 0xff));
}

void vga_wait_for_vblank(void) {
  // wait for any current vblank to end
  while (inb(VGA_STATUS) & 0x08);
  // wait for the next one 
  while (!(inb(VGA_STATUS) & 0x08));
}

void vga_print_char_colour(u8 c, u8 colour) {
  if (c == '\n') {
    __vga_current_y++;
    if (__vga_current_y == VGA_ROWS) {
      vga_scroll(D_UP, colour);
      __vga_current_y--;
    } 
    __vga_current_x = 0;
    vga_set_cursor_position(__vga_current_x, __vga_current_y);
    return;
  }
  screen_buf[__vga_current_y][__vga_current_x] = (VGA_Slot){c, colour};
  
  __vga_current_x++;
  if (__vga_current_x >= VGA_COLS) {
    __vga_current_y++;
    if (__vga_current_y == VGA_ROWS) {
      vga_scroll(D_UP, colour);
      __vga_current_y--;
    } 
    __vga_current_x = 0;
  }
  vga_set_cursor_position(__vga_current_x, __vga_current_y);
}

void vga_remove_character(void) {
  __vga_current_x--;
  screen_buf[__vga_current_y][__vga_current_x].c = 0;
  if (__vga_current_x <= 0) {
    __vga_current_x = VGA_COLS - 1;
    __vga_current_y--;

    if (__vga_current_y < 0) {
      __vga_current_y = 0;
    }
  }
  while (screen_buf[__vga_current_y][__vga_current_x].c == 0 && __vga_current_x >= 0) {
      __vga_current_x--;
  }
  __vga_current_x++;
  vga_set_cursor_position(__vga_current_x, __vga_current_y);
}

void vga_print_string_colour(char* s, u8 colour) {
  int l = strlen(s);
  for (int i = 0; i < l; ++i) {
    vga_print_char_colour(s[i], colour);
  }
}

void vga_flip_buffer(void) {
  vga_wait_for_vblank();
  for (int y = 0; y < VGA_ROWS; ++y) {
    for (int x = 0; x < VGA_COLS; ++x) {
      vga_mem[y][x] = screen_buf[y][x];
      if (!screen_buf[y][x].c) {
        vga_mem[y][x].c = ' ';
      }
    }
  }
}

void vga_scroll(Direction d, u8 colour) {
  if (d == D_UP) {
    for (int y = 0; y < VGA_ROWS; ++y) {
      for (int x = 0; x < VGA_COLS; ++x) {
        screen_buf[y][x] = screen_buf[y+1][x];
      }
    }
    for (int x = 0; x < VGA_COLS; ++x) {
      screen_buf[VGA_ROWS-1][x] = (VGA_Slot){0, colour};
    }
  } else {
    for (int y = 1; y < VGA_ROWS; ++y) {
      for (int x = 0; x < VGA_COLS; ++x) {
        screen_buf[y-1][x] = screen_buf[y][x];
      }
    }
    for (int x = 0; x < VGA_COLS; ++x) {
      screen_buf[0][x] = (VGA_Slot){0, colour};
    }
  }
}