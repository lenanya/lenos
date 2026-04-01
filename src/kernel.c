#include "common.h"
#include "vga.h"
#include "keyboard.h"
#include "std/string.h"
#include "std/da.h"
#include "std/file.h"
#include "lfs.h"

void kernel_main() {
  vga_clear_screen(VGA_CYAN_ON_GREY);
  vga_flip();

  File_Buffer fb = {0};
  read_entire_file(&fb, "test.txt");

  if (!fb.exists) {
    vga_puts_colour("test.txt does not exist", VGA_RED_ON_GREY);
    vga_flip();
    while (1);
  } 
  
  vga_puts_colour(fb.items, VGA_CYAN_ON_GREY);
  vga_flip();

  fb.size = 0;
  read_entire_file(&fb, "test2.txt");

  if (!fb.exists) {
    vga_puts_colour("test2.txt does not exist", VGA_RED_ON_GREY);
    vga_flip();
    while (1);
  } 
  
  vga_puts_colour(fb.items, VGA_CYAN_ON_GREY);
  vga_flip();

  while (1);
}