#include "ata.h"
#include "common.h"
#include "std/da.h"
#include "std/io.h"

void ata_wait_for_ready(void) {
  // while busy
  while (inb(ATA_CMD_STAT) & ATA_BUSY);
  // while not ready
  while (!(inb(ATA_CMD_STAT) & ATA_READY));
}

void ata_read_sectors(u32 lba, u8 count, u16* dest) {
  outb(ATA_DEVICE_LBA, (0xe0 | ((lba >> 24) & 0x0f)));
  outb(ATA_SEC_COUNT, count);
  outb(ATA_LBA_LOW, (u16)lba);
  outb(ATA_LBA_MID, (u16)(lba >> 8));
  outb(ATA_LBA_HIGH, (u16)(lba >> 16));
  outb(ATA_CMD_STAT, ATA_READ);

  for (i32 i = 0; i < count; ++i) {
    ata_wait_for_ready();
    for (i32 j = 0; j < 256; ++j) {
      dest[i * 256 + j] = inw(ATA_DATA);
    }
  }
}

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