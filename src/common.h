#ifndef COMMON_H
#define COMMON_H

#define false (bool)0
#define true (bool)1

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef char bool;
typedef unsigned int uint;
typedef unsigned long long ulong;

void outb(ushort port, uchar val);
uchar inb(ushort port);
ushort inw(ushort port);

#endif //COMMON_H