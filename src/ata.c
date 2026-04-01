#include "ata.h"
#include "common.h"
#include "std/da.h"

void ata_wait_for_ready(void) {
  // while busy
  while (inb(ATA_CMD_STAT) & ATA_BUSY);
  // while not ready
  while (!(inb(ATA_CMD_STAT) & ATA_READY));
}

void ata_read_sectors(uint lba, uchar count, ushort* dest) {
  outb(ATA_DEVICE_LBA, (0xe0 | ((lba >> 24) & 0x0f)));
  outb(ATA_SEC_COUNT, count);
  outb(ATA_LBA_LOW, (ushort)lba);
  outb(ATA_LBA_MID, (ushort)(lba >> 8));
  outb(ATA_LBA_HIGH, (ushort)(lba >> 16));
  outb(ATA_CMD_STAT, ATA_READ);

  for (int i = 0; i < count; ++i) {
    ata_wait_for_ready();
    for (int j = 0; j < 256; ++j) {
      dest[i * 256 + j] = inw(ATA_DATA);
    }
  }
}