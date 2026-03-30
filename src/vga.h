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

#define VGA_COLS 80
#define VGA_ROWS 25

typedef struct {
  uchar c;
  uchar colour;
} VGA_Slot;

void vga_set_cursor_position(int x, int y);
void vga_wait_for_vblank(void);
void vga_disable_cursor(void);
void vga_enable_cursor(void);
void vga_clear_screen(uchar colour);
void vga_putc_colour(uchar c, uchar colour);
void vga_puts_colour(char* s, uchar colour);
#define vga_putc(c) vga_putc_colour(c, VGA_WHITE_ON_BLACK) 
#define vga_puts(s) vga_puts_colour(s, VGA_WHITE_ON_BLACK) 
void vga_delc(void);

#endif // VGA_H