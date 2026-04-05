#include "file.h"
#include "../lfs.h"
#include "../ata.h"
#include "da.h"
#include "../vga.h"
#include "string.h"

void read_entire_file(File_Buffer* fb, char* filename) {
  LFS_Table_Entry* te = lfs_find_file(filename);
  if (!te) {  
    fb->exists = false;
    return;
  } else {
    fb->exists = true;
  }

  u16* block_buf = (u16*)malloc(512);
  u32 lba = te->file_first_lba;
  while (lba != 0) {
    ata_read_sectors(lba, 1, block_buf);
    LFS_File_Block* fbl = (LFS_File_Block*)block_buf;
    lba = fbl->next_block_lba;
    for (u32 i = 0; i < 507 && fb->size < te->file_size; ++i) {
      da_append(fb, fbl->data[i]);
    }
  }
  free(te);
  free(block_buf);
}

void write_entire_file(File_Buffer* fb, char* filename) {
  u32 file = lfs_find_first_free();
  if (!file) return;
  u32 table = lfs_find_first_free_table_block();
  if (!table) return;
  LFS_Table_Entry* ent = malloc(sizeof(LFS_Table_Entry));
  ent->deleted = false;
  ent->file_first_lba = file;
  ent->file_size = fb->size;
  ent->flags = 0;
  ent->last = 1;

  for (int i = 0; i < strlen(filename); ++i) {
    ent->name[i] = filename[i];
  }

  lfs_append_table(ent);

  LFS_Superblock* sb = lfs_get_superblock();
  sb->entry_count++;
  lfs_write_superblock(sb);

  LFS_File_Block* fbl = malloc(sizeof(LFS_File_Block));

  if (fb->size % 512 != 0) {
    fbl->flags = BLOCK_USED;
    fbl->next_block_lba = 0;
    for (u32 i = 0; i < 507; ++i) {
      if (i >= fb->size) fbl->data[i] = 0;
      else fbl->data[i] = fb->items[i];
    }
    ata_write_sector(file, (u16*)fbl);
    return;
  }
}

void file_buffer_free(File_Buffer* fb) {
  free(fb->items);
}

