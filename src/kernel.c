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
  String_Buffer cmd = {0};
  for (;;) {
    cmd.size = 0;
    vga_puts("> ");
    vga_flip();
    uchar sc;
    do {
      sc = kb_get_scancode();
      if (!kb_is_make(sc)) continue;
      if (sc == 14) {
        if (cmd.size > 0) {
          cmd.size--;
          vga_delc();
          vga_flip();
        }
      }
      uchar ascii = kb_gb_map[sc];
      if (ascii) {
        da_append(&cmd, ascii);
        vga_putc(ascii);
        vga_flip();
      }
    } while (sc != 28);
    cmd.size--; // remove newline
    da_append(&cmd, 0);
    if (cmd.size < 1) continue;
    char* args = cmd.items;
    while (args[0] && args[0] != ' ') args++;
    if (args[0] == ' ') {
      args[0] = 0;
      args++; // skip null
    }

    if (strcmp(cmd.items, "echo") == true) {
      vga_putc(' ');
      if (args[0]) vga_puts(args);
      vga_putc(10);
      vga_flip();
    } else if (strcmp(cmd.items, "cat") == true) {
      
      *(uint*)0x5000 = (uint)args;

      if (!args[0]) {
        vga_putc(' ');
        vga_puts_colour("ERROR: No file specified\n", VGA_RED_ON_GREY);
        vga_flip();
        continue;
      }
      File_Buffer fb = {0};
      read_entire_file(&fb, args);
      da_append(&fb, 0);

      if (!fb.exists) {
        vga_puts_colour(" ERROR: ", VGA_RED_ON_GREY);
        vga_puts_colour(args, VGA_RED_ON_GREY);
        vga_puts_colour(" does not exist\n", VGA_RED_ON_GREY);
        vga_flip();
      } else {
        vga_putc(' ');
        vga_puts(fb.items);
        vga_putc(10);
        vga_flip();
      }
    } else if (strcmp(cmd.items, "ls") == true) {
      Directory dir = {0};
      lfs_read_directory(&dir);

      for (int i = 0; i < dir.size; ++i) {
        vga_putc(' ');
        vga_puts(dir.items[i]->name);
        vga_puts("      ");
        vga_puts(itoa(dir.items[i]->file_size));
        vga_putc('B');
        vga_putc(10);
        vga_flip();
      }
    } else if (strcmp(cmd.items, "clear") == true) {
      vga_clear_screen(VGA_CYAN_ON_GREY);
      vga_flip();
    }else {
      vga_puts_colour(" ERROR: Unknown command: ", VGA_RED_ON_GREY);
      vga_puts_colour(cmd.items, VGA_RED_ON_GREY);
      vga_putc_colour(10, VGA_RED_ON_GREY);
    }
  }
}