#ifndef VGA_H
#define VGA_H

#include "common.h"

#define VGA_MEM          0xb8000
#define VGA_INDEX        0x3d4
#define VGA_DATA         0x3d5
#define VGA_STATUS       0x3da
#define VGA_CURSOR_H     0x0e
#define VGA_CURSOR_L     0x0f
#define VGA_CURSOR_START 0x0a
 
#define VGA_CYAN_ON_BLACK  0x03
#define VGA_CYAN_ON_GREY   0x83
#define VGA_RED_ON_BLACK   0x04
#define VGA_WHITE_ON_BLACK 0x0f
#define VGA_RED_ON_GREY    0x84

#define VGA_COLS 80
#define VGA_ROWS 25

typedef struct {
  uchar c;
  uchar colour;
} VGA_Slot;

typedef enum {
  D_UP,
  D_DOWN
} Direction;

typedef VGA_Slot VGA_Row[80];

void vga_set_cursor_position(int x, int y);
void vga_wait_for_vblank(void);
void vga_disable_cursor(void);
void vga_enable_cursor(void);
void vga_clear_screen(uchar colour);
void vga_putc_colour(uchar c, uchar colour);
void vga_puts_colour(char* s, uchar colour);
#define vga_putc(c) vga_putc_colour(c, VGA_CYAN_ON_GREY) 
#define vga_puts(s) vga_puts_colour(s, VGA_CYAN_ON_GREY) 
void vga_delc(void);
void vga_flip(void);
void vga_scroll(Direction d, uchar colour);

#endif // VGA_H