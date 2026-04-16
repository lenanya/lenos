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
  if (!lb) return 20;
  lb->capacity = 0;
  lb->size = 0;
  lb->items = NULL;
  char* filename = NULL;

  char c = 0;
  String_Buffer* line = (String_Buffer*)std.mem.malloc(sizeof(String_Buffer));
  if (!line) return 20;
  for (;;) {
    while (c != '\n') {
      c = std.io.getc();
      if (c != '\n') da_append(line, c);
      std.io.putc(c);
      if (c == '\n') {
        da_append(line, 0);
        char* cmd  = line->items;
        char* rest = line->items;
        while (rest[0] != ' ' && rest[0] != 0) {
          std.io.printf("%p\n", rest);
          ++rest;
        }
        if (rest[0] == ' ') {
          rest[0] = 0;
          ++rest;
        }

        std.io.printf("cmd: %p, rest: %p\n", cmd, rest);
        //std.mem.heap_dump();
        //return 30;

        if (std.str.strcmp(cmd, ":p") == true) {
          if (rest[0] != 0) {
            if (std.str.s_is_digits(rest)) {
              u32 l = std.str.stou32(rest);
              if (l < lb->size) {
                std.io.printf("%s\n", lb->items[l]->items);
              } else {
                std.io.printf("p: Line %u doesn't exist\n", l);
              }
            } else {
              std.io.printf("?\n");
            }
          }
        } else if (std.str.strcmp(cmd, ":a") == true) {
          if (rest[0] != 0) {
            String_Buffer* temp = (String_Buffer*)std.mem.malloc(sizeof(String_Buffer));
            if (!temp) return 20;
            for (u32 i = 0; i < std.str.strlen(rest); ++i) {
              da_append(temp, rest[i]);
            }
            da_append(temp, '\n');
            da_append(temp, 0);
            da_append(lb, temp);
          } else {
            std.io.printf("a: No text supplied\n");
          }
        } else {
          std.io.printf("?\n");
        }


      }
    }
    c = 0;
    line->size = 0;
  }
  for (u32 i = 0; i < lb->size; ++i) {
    std.mem.free(lb->items[i]->items);
    std.mem.free(lb->items[i]);
  }
  std.mem.free(lb->items);
  std.io.printf("end");
  return 0;
}