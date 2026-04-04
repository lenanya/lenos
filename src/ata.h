#ifndef ATA_H
#define ATA_H
#include "common.h"

#define ATA_CMD_STAT   0x1F7
#define ATA_DATA       0x1F0
#define ATA_SEC_COUNT  0x1F2
#define ATA_LBA_LOW    0x1F3
#define ATA_LBA_MID    0x1F4
#define ATA_LBA_HIGH   0x1F5
#define ATA_DEVICE_LBA 0x1F6


#define ATA_READ   0x20
#define ATA_WRITE  0x30
#define ATA_BUSY   0x80
#define ATA_READY  0x08

void ata_wait_for_ready(void);
void ata_read_sectors(u32 lba, u8 count, u16* dest);
void ata_write_sector(u32 lba, u16* data);

#endif // ATA_H