#ifndef COMMON_H
#define COMMON_H

typedef unsigned char uchar;
typedef unsigned short ushort;

void outb(ushort port, uchar val);
uchar inb(ushort port);

#endif //COMMON_H