#include "common.h"
#include "std.h"

#include "kernel/ata.h"
#include "kernel/keyboard.h"
#include "kernel/lfs.h"
#include "kernel/vga.h"

#include "std/io.h"
#include "std/mem.h"

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
}