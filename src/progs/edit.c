#include "../std.h"
#include "prog_da.h"

#define HEIGHT 25
#define WIDTH  80

Std std = {0};

typedef struct Line_Buffer {
  String_Buffer** items;
  u32 size;
  u32 capacity;
} Line_Buffer;

i32 main(Std s, Args args) {
  std = s;
  Line_Buffer* lb = std.mem.malloc(sizeof(Line_Buffer));
  lb->capacity = 0;
  lb->size = 0;
  lb->items = NULL;
  char* filename = NULL;

  std.io.printf("%u\n", args.size);

  if (args.size > 0) {
    LFS_Table_Entry* te = std.file.get_file_entry(args.items[0]);
    if (te) {
      filename = std.mem.memdup(te->name, 32);
      if (te->file_size > 1) {
        File_Buffer data = {0};
        std.file.read_entire_file(&data, filename);
        da_append(&data, 0);
        u32 i = 0;
        while (i < data.size && data.items[i] != 0) {
          String_Buffer* current = std.mem.malloc(sizeof(String_Buffer));
          current->items = NULL;
          current->size = 0;
          current->capacity = 0;
          while (i < data.size && data.items[i] != 0) {
            if (data.items[i] == '\n') {
              da_append(current, '\n');
              ++i;
              break;
            }
            if (data.items[i] == 0) {
              i = data.size;
              break;
            }
            std.io.putc(data.items[i]);
            da_append(current, data.items[i]);
            ++i;
          }
          da_append(current, 0);
          da_append(lb, current);
          ++i;
        }
        std.mem.free(data.items);
      }
    }
  }

  // return to see if we get here
  return 20;

  char c = 0;
  String_Buffer line = {0};
  for (;;) {
    while (c != '\n') {
      c = std.io.getc();
      da_append(&line, c);
      std.io.putc(c);
      if (c == '\n') {
        da_append(&line, 0);
        char* cmd  = line.items;
        char* rest = line.items;
        while (rest[0] != ' ' && rest[0] != 0) ++rest;
        if (rest[0] == ' ') {
          rest[0] = 0;
          ++rest;
        }
        if (std.str.s_is_digits(cmd)) {
          u32 l = std.str.stou32(cmd);
          std.io.printf("%u\n", l);
        }
        // TODO: handle line

      }
    }
    line.size = 0;
  }
  for (u32 i = 0; i < lb->size; ++i) {
    std.mem.free(lb->items[i]->items);
    std.mem.free(lb->items[i]);
  }
  std.mem.free(lb->items);
  std.io.printf("end");
  return 0;
}