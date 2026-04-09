#ifndef VGA_H
#define VGA_H

#include "../common.h"

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
  u8 c;
  u8 colour;
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
void vga_clear_screen(u8 colour);
void vga_print_char_colour(u8 c, u8 colour);
void vga_print_string_colour(char* s, u8 colour);
#define vga_print_char(c) vga_print_char_colour(c, VGA_CYAN_ON_GREY) 
#define vga_print_string(s) vga_print_string_colour(s, VGA_CYAN_ON_GREY) 
void vga_remove_character(void);
void vga_flip_buffer(void);
void vga_scroll(Direction d, u8 colour);

#endif // VGA_H