#ifndef COMMON_H
#define COMMON_H

#define false (bool)0
#define true (bool)1

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef char bool;

void outb(ushort port, uchar val);
uchar inb(ushort port);

#endif //COMMON_H