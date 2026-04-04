#include "io.h"

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