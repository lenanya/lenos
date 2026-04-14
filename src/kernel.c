#include "common.h"

#include "kernel/vga.h"
#include "kernel/keyboard.h"
#include "kernel/lfs.h"

#include "std.h"

#include "kernel_std/mem.h"
#include "kernel_std/string.h"
#include "kernel_std/da.h"
#include "kernel_std/file.h"
#include "kernel_std/io.h"

#include "progs/helloworld_contents.h"
#include "progs/edit_contents.h"

#define PROGRAM_ADDR (addr)0x90000

void add_prog(u8* data, u32 size, char* filename);

void kernel_main() {
  vga_clear_screen(VGA_CYAN_ON_GREY);
  vga_flip_buffer();
  String_Buffer cmd = {0};
  Directory dir = {0};
  i32 ret_code = 0;

  add_prog(helloworld_contents, helloworld_length, "hw.x");
  add_prog(edit_contents, edit_length, "edit");

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
      if (cmd.size >= 72) continue;
      if (ascii) {
        da_append(&cmd, ascii);
        putc(ascii);
      }
    } while (sc != 28);
    vga_flip_buffer();
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
      if (strcmp(args, "//") == true) {
        printf("%d\n", ret_code);
      } else if (args[0]) {
         println(args);
      } else println("");
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
        eprintln(" does not exist");
      } else {
        print(" ");
        println(fb.items);
      }
      free(fb.items);
    } else if (strcmp(cmd.items, "ls") == true) {
      dir.size = 0;
      lfs_read_directory(&dir);

      for (u32 i = 0; i < dir.size; ++i) {
        printf(" %s   %dB\n", dir.items[i]->name, dir.items[i]->file_size);
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
      for (u32 i = 0; i < strlen(args); ++i) {
        da_append(&fb, args[i]);
      }
      write_entire_file(&fb, name);
      free(fb.items);
    } else if (strcmp(cmd.items, "dump") == true) {
      heap_dump();
    } else if (strcmp(cmd.items, "big") == true) {
      File_Buffer fb = {0};
      for (u32 j = 0; j < 3; ++j) {
        for (u32 i = 0; i < 507; ++i) {
          da_append(&fb, '1' + j);
        }
      }
      write_entire_file(&fb, "test3.txt");
      free(fb.items);
    } else if (strcmp(cmd.items, "reboot") == true) {
      // this reboots computer for some reason :3
      outb(KB_CTRL, 0xFE);
    } else if (strcmp(cmd.items, "rm") == true) {
      if (!args[0]) {
        eprintln("ERROR: No file specified");
        continue;
      }
      lfs_delete_file(args);
    } else {
      LFS_Table_Entry* te = lfs_find_file(cmd.items);
      if (!te) {
        eprint(" ERROR: Unknown command: ");
        eprintln(cmd.items);
        free(te);
        continue;
      }
      free(te);
      File_Buffer fb = {0};
      read_entire_file(&fb, cmd.items);
      memncpy(fb.items, (addr)PROGRAM_ADDR, fb.size);
      free(fb.items);
      i32(*main)(Std std, Args args) = (i32(*)(Std, Args))PROGRAM_ADDR;
      Std std = {0};
      init_std(&std);
      Args user_args = {0};
      while (args[0] != 0) {
        da_append(&user_args, args);
        while (args[0] != ' ' && args[0] != 0) {
          args++;
        }
        if (args[0] == ' ') {
          args[0] = 0;
          args++;
        }
      }
      ret_code = main(std, user_args);
      free_user_allocations();
      free(user_args.items);
    }
    for (u32 i = 0; i < dir.size; ++i) {
      free(dir.items[i]);
    }
  }
}

void add_prog(u8* data, u32 size, char* filename) {
  LFS_Table_Entry* te = lfs_find_file(filename);
  if (!te) {
    File_Buffer fb = {0};
    for (u32 i = 0; i < size; ++i) {
      da_append(&fb, data[i]);
    }
    write_entire_file(&fb, filename);
    free(fb.items);
  }
  free(te);
}