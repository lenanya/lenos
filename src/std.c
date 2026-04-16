#include "common.h"
#include "std.h"

#include "kernel/ata.h"
#include "kernel/keyboard.h"
#include "kernel/lfs.h"
#include "kernel/vga.h"

#include "kernel_std/io.h"
#include "kernel_std/mem.h"
#include "kernel_std/file.h"
#include "kernel_std/string.h"

void init_std(Std* std) {
  std->io = (Std_Io) {
    .printf = printf,
    .print = print,
    .println = println,
    .putc = putc,
    .getc = getc,
  };
  std->mem = (Std_Mem) {
    .calloc = calloc,
    .free = free,
    .malloc = user_malloc,
    .memdup = memdup,
    .memncpy = memncpy,
    .realloc = realloc,
    .heap_dump = heap_dump,
  };
  std->file = (Std_File) {
    .file_buffer_free = file_buffer_free,
    .read_entire_file = read_entire_file,
    .write_entire_file = write_entire_file,
    .delete_file = delete_file,
    .get_file_entry = lfs_find_file,
  };
  std->str = (Std_String) {
    .utoa = utoa,
    .strlen = strlen,
    .stou32 = stou32,
    .is_digit = is_digit,
    .s_is_digits = s_is_digits,
    .strcmp = strcmp,
  };
}