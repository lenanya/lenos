#include "common.h"
#include "vga.h"
#include "keyboard.h"
#include "lfs.h"

#include "std/string.h"
#include "std/da.h"
#include "std/file.h"
#include "std/io.h"

void kernel_main() {
  vga_clear_screen(VGA_CYAN_ON_GREY);
  vga_flip_buffer();
  String_Buffer cmd = {0};
  Directory dir = {0};

  for (;;) {
    cmd.size = 0;
    print(" > ");
    u8 sc;
    do {
      sc = kb_get_scancode();
      if (!kb_is_make(sc)) continue;
      if (sc == 14) {
        if (cmd.size > 0) {
          cmd.size--;
          vga_remove_character();
          vga_flip_buffer();
        }
      }
      u8 ascii = kb_gb_map[sc];
      if (ascii) {
        da_append(&cmd, ascii);
        putc(ascii);
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
      print(" ");
      if (args[0]) println(args);
      else println("");
    } else if (strcmp(cmd.items, "cat") == true) {
      if (!args[0]) {
        eprintln(" ERROR: No file specified");
        continue;
      }
      File_Buffer fb = {0};
      read_entire_file(&fb, args);
      da_append(&fb, 0);

      if (!fb.exists) {
        eprint(" ERROR: ");
        eprint(args);
        eprintln(" does not exist\n");
      } else {
        print(" ");
        println(fb.items);
      }
    } else if (strcmp(cmd.items, "ls") == true) {
      dir.size = 0;
      lfs_read_directory(&dir);

      for (int i = 0; i < dir.size; ++i) {
        print(" ");
        print(dir.items[i]->name);
        print("      ");
        print(itoa(dir.items[i]->file_size));
        println("B");
      }
    } else if (strcmp(cmd.items, "clear") == true) {
      vga_clear_screen(VGA_CYAN_ON_GREY);
      vga_flip_buffer();
    } else if (strcmp(cmd.items, "write") == true) {
      char* name = args;
      while (args[0] != ' ' && args < cmd.items+cmd.size) args++;
      args[0] = 0;
      args++;
      File_Buffer fb = {0};
      for (int i = 0; i < strlen(args); ++i) {
        da_append(&fb, args[i]);
      }
      write_entire_file(&fb, name);
    } else {
      eprint(" ERROR: Unknown command: ");
      eprintln(cmd.items);
    }
  }
}