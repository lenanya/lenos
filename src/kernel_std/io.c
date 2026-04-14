#include "io.h"
#include "stdarg.h"
#include "string.h"
#include "mem.h"
#include "../kernel/vga.h"
#include "../kernel/keyboard.h"

void println(char* s) {
  vga_print_string(s);
  vga_print_char(10);
  vga_flip_buffer();
}

void print(char* s) {
  vga_print_string(s);
  vga_flip_buffer();
}

void eprintln(char* s) {
  vga_print_string_colour(s, VGA_RED_ON_GREY);
  vga_print_char(10);
  vga_flip_buffer();
}

void eprint(char* s) {
  vga_print_string_colour(s, VGA_RED_ON_GREY);
  vga_flip_buffer();
}

void putc(char c) {
  vga_print_char(c);
  vga_flip_buffer();
}

void printf(char* fmt, ...) {
  va_list args; 
  u32 total = 0;
  va_start(args, fmt);

  u32 l = strlen(fmt);
  u32 i = 0;
  while (i < l) {
    char c = fmt[i];
    if (c == '%') {
      ++i;
      if (i >= l) {
        va_end(args);
        vga_flip_buffer();
        return;
      }
      c = fmt[i];
      switch (c) {
        case 'd': {
          i32 arg = va_arg(args, i32);
          char* temp = itoa(arg);
          print(temp);
          free(temp);
          break;
        }
        case 'u': {
          u32 arg = va_arg(args, u32);
          char* temp = utoa(arg);
          print(temp);
          free(temp);
          break;
        }
        case 's': {
          char* arg = va_arg(args, char*);
          print(arg);
          break;
        }
        case 'c': {
          i32 arg = va_arg(args, i32);
          putc(arg);
          break;
        }
        case 'b': {
          u8 arg = (u8)va_arg(args, i32);
          if (arg) {
            print("true");
          } else {
            print("false");
          }
          break;
        }
        case 'p': {
          void* arg = va_arg(args, void*);
          char* temp = u32_to_hex((u32)arg);
          print(temp);
          free(temp);
          break;
        }
        default: {
          va_end(args);
          vga_flip_buffer();
          return;
        }
      }
    } else {
      putc(c);
    }
    ++i;
  }
  va_end(args);
  vga_flip_buffer();
}

char getc() {
  u8 sc = 0;
  bool shift = false;
  for (;;) {
    sc = kb_get_scancode();
    if (!kb_is_make(sc)) {
      if (sc == 42) shift = false;
      continue;
    }
    if (sc == 42) shift = true;
    u8 ascii;
    if (shift) {
      ascii = kb_gb_shift_map[sc];
    } else {
      ascii = kb_gb_map[sc];
    }
    if (ascii) return ascii;
  }
}