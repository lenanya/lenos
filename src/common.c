#include "common.h"

void outb(ushort port, uchar val) {
  asm volatile ("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

uchar inb(ushort port) {
  uchar res;
  asm volatile ("inb %w1, %b0" : "=a"(res) : "Nd"(port) : "memory");
  return res;
}