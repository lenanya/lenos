#include "../std.h"

Std std = {0};

i32 main(Std s, Args args) {
  std = s;

  for (u32 i = 0; i < args.size; ++i) {
    std.io.printf("%s\n", args.items[i]);
  }

  return 0;
}