#include "common.h"
#include "std.h"

#include "kernel/ata.h"
#include "kernel/keyboard.h"
#include "kernel/lfs.h"
#include "kernel/vga.h"

#include "kernel_std/io.h"
#include "kernel_std/mem.h"
#include "kernel_std/file.h"

void init_std(Std* std) {
  std->io = (Std_Io) {
    .printf = printf,
    .print = print,
    .println = println,
    .putc = putc,
  };
  std->mem = (Std_Mem) {
    .calloc = calloc,
    .free = free,
    .malloc = user_malloc,
    .memdup = memdup,
    .memncpy = memncpy,
    .realloc = realloc,
  };
  std->file = (Std_File) {
    .file_buffer_free = file_buffer_free,
    .read_entire_file = read_entire_file,
    .write_entire_file = write_entire_file,
    .delete_file = delete_file,
  };
}