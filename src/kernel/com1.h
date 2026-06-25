#ifndef COM1_H
#define COM1_H
#include "../common.h"

// data register
#define COM1_DATA 0x3F8
// interrupt enable register
#define COM1_IER  0x3F9
// fifo control register
#define COM1_FCR 0x3FA
// line control register
#define COM1_LCR  0x3FB
// modem control register
#define COM1_MCR  0x3FC
// line status register
#define COM1_LSR  0x3FD


/* 
baudrate divisor high and low byte
0x0001 since i want 115200 which is the default

for future reference, to change baudrate:
divide 115200 by the desired rate and write the result
example, if you want 38400:
115200/38400 = 3
so you write 0x0003
*/
#define COM1_BR_DIV_L 0x01
#define COM1_BR_DIV_H 0x00

#define COM1_INTERRUPT_DISABLE 0


/*
DLAB: Divisor Latch Access Bit in the LCR
needed to let you use the data register 
to configure the baudrate
*/
#define COM1_ENABLE_DLAB 0x80

/*
data format
- 8 bits
- no parity
- 1 stop bit
*/
#define COM1_FORMAT_8N1 0x03

/*
FIFO configuration
- fifo enabled
- clear receiver and transmitter circuits
- DMA mode 0 (single transfer mode)
- interrupt trigger threshhold of 14 bytes
*/
#define COM1_FIFO_CONFIG 0xC7

/*
modem configuration
- data terminal ready
- request to send
- auxiliary output 2
*/
#define COM1_MODEM_CONFIG 0x0B

// bit of the LSR to check if ready to transmit
#define COM1_TRANS_HOLD_EMPTY 0x20

void com1_init();
void com1_putc(char c);

#endif // COM1_H