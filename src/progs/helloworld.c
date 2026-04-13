#include "../std.h"

Std std = {0};

i32 main(Std s, Args args) {
  std = s;
  std.io.printf("Hello, World!\n");
  return 0;
}