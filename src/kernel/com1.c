#include "com1.h"

// initialise UART to be ready to use serial
void com1_init() {
  // disable interrupts
  outb(COM1_IER, COM1_INTERRUPT_DISABLE);

  // set DLAB to send baudrate divisor
  outb(COM1_LCR, COM1_ENABLE_DLAB);

  // send baudrate divisor
  outb(COM1_DATA, COM1_BR_DIV_L);
  outb(COM1_IER, COM1_BR_DIV_H);

  // unset DLAB and set data format
  outb(COM1_LCR, COM1_FORMAT_8N1);

  // setup fifo
  outb(COM1_FCR, COM1_FIFO_CONFIG);

  // ready modem
  outb(COM1_MCR, COM1_MODEM_CONFIG);
}

// send a byte over serial
void com1_putc(char c) {
  // wait for transmit holding register to be empty
  while (!(inb(COM1_LSR) & COM1_TRANS_HOLD_EMPTY));

  // send the byte
  outb(COM1_DATA, c);
}