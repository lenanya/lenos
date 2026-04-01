#include "file.h"
#include "../lfs.h"
#include "../ata.h"
#include "da.h"
#include "../vga.h"

void read_entire_file(File_Buffer* fb, char* filename) {
  LFS_Table_Entry* te = lfs_find_file(filename);
  if (!te) {
    fb->exists = false;
  } else {
    fb->exists = true;
  }

  ushort* block_buf = (ushort*)malloc(512);
  uint lba = te->file_first_lba;
  while (lba != 0) {
    ata_read_sectors(lba, 1, block_buf);
    LFS_File_Block* fbl = (LFS_File_Block*)block_buf;
    lba = fbl->next_block_lba;

    for (uint i = 0; i < 508 && fb->size < te->file_size; ++i) {
      da_append(fb, fbl->data[i]);
    }
  }
}

void file_buffer_free(File_Buffer* fb) {
  free(fb->items);
}

