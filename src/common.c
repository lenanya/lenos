#include "common.h"

void outb(u16 port, u8 val) {
 __asm__ volatile ("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

u8 inb(u16 port) {
  u8 res;
 __asm__ volatile ("inb %w1, %b0" : "=a"(res) : "Nd"(port) : "memory");
  return res;
}

u16 inw(u16 port) {
  u16 ret;
 __asm__ volatile ("inw %w1, %w0" : "=a"(ret) : "Nd"(port));
  return ret;
}

void outw(u16 port, u16 w) {
 __asm__ volatile ("outw %w0, %w1" : : "a"(w), "Nd"(port) : "memory");
}