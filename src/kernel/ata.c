#include "ata.h"
#include "../common.h"
#include "../kernel_std/da.h"
#include "../kernel_std/io.h"

// wait for the hard drive to become ready
void ata_wait_for_ready(void) {
  // while busy
  while (inb(ATA_CMD_STAT) & ATA_BUSY);
  // while not ready
  while (!(inb(ATA_CMD_STAT) & ATA_READY));
}

// read the sector at lba into dest
// TODO: fix magic numbers
void ata_read_sector(u32 lba, u16* dest) {
  outb(ATA_DEVICE_LBA, (0xe0 | ((lba >> 24) & 0x0f)));
  outb(ATA_SEC_COUNT, 1);
  outb(ATA_LBA_LOW, (u16)lba);
  outb(ATA_LBA_MID, (u16)(lba >> 8));
  outb(ATA_LBA_HIGH, (u16)(lba >> 16));
  outb(ATA_CMD_STAT, ATA_READ);

  ata_wait_for_ready();
  for (i32 j = 0; j < 256; ++j) {
    dest[i * 256 + j] = inw(ATA_DATA);
  }
}

// write the bytes in data to the block at lba
// TODO: fix magic numbers
void ata_write_sector(u32 lba, u16* data) {
  outb(ATA_DEVICE_LBA, (0xe0 | ((lba >> 24) & 0x0f)));
  outb(ATA_SEC_COUNT, 1);
  outb(ATA_LBA_LOW, (u16)lba);
  outb(ATA_LBA_MID, (u16)(lba >> 8));
  outb(ATA_LBA_HIGH, (u16)(lba >> 16));

  outb(ATA_CMD_STAT, ATA_WRITE);

  while (!(inb(ATA_CMD_STAT) & ATA_READY));

  for (i32 i = 0; i < 256; ++i) {
    outw(ATA_DATA, data[i]);
  }

  outb(ATA_CMD_STAT, 0xe7);

  while (inb(ATA_CMD_STAT) & ATA_BUSY);
}