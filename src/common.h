#ifndef COMMON_H
#define COMMON_H

#define false (bool)0
#define true (bool)1

typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef char bool;
typedef int i32;
typedef unsigned int u32;
typedef long long i64;
typedef unsigned long long u64;
typedef u8* addr;

typedef struct File_Buffer {
  char* items;
  u32 size;
  u32 capacity;
  bool exists;
} File_Buffer;

typedef enum {
  FILE,
} File_Type;

void outb(u16 port, u8 val);
u8 inb(u16 port);
u16 inw(u16 port);
void outw(u16 port, u16 w);

#endif //COMMON_H