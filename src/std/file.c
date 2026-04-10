#include "file.h"
#include "../kernel/lfs.h"
#include "../kernel/ata.h"
#include "da.h"
#include "../kernel/vga.h"
#include "string.h"
#include "io.h"

void read_entire_file(File_Buffer* fb, char* filename) {
  fb->size = 0;
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
  LFS_Table_Entry* te = lfs_find_file(filename);
  if (te) {
    lfs_delete_file(filename);
  } 
  u32 file = lfs_find_first_free();
  if (!file) return;
  LFS_Table_Position tp = lfs_find_first_free_table_block();
  if (!tp.lba) return;
  LFS_Table_Entry* ent = calloc(sizeof(LFS_Table_Entry), 0);
  ent->deleted = false;
  ent->file_size = fb->size;
  ent->flags = 0;

  for (u32 i = 0; i < strlen(filename) && i < 32; ++i) {
    ent->name[i] = filename[i];
  }

  LFS_File_Block* fbl = malloc(sizeof(LFS_File_Block));
  if (fb->size <= 507) {
    fbl->flags = BLOCK_USED;
    fbl->next_block_lba = 0;
    for (u32 i = 0; i < 507; ++i) {
      if (i >= fb->size) fbl->data[i] = 0;
      else fbl->data[i] = fb->items[i];
    }
    ata_write_sector(file, (u16*)fbl);
    free(fbl);
  } else {
    u32 padding = 507 - (fb->size % 507);
    u32 block_count = (fb->size + padding) / 507;
    i32 block_index = block_count - 2;
    u32 prev_block = 0;
    while (block_index >= 0) {
      u32 start = block_index * 507;
      fbl->flags = BLOCK_USED;
      fbl->next_block_lba = prev_block;
      prev_block = file;
      for (u32 i = 0; i < 507; ++i) {
        if (start + i >= fb->size) {
          fbl->data[i] = 0;
        } else {
          fbl->data[i] = fb->items[start + i];
        }
      }
      ata_write_sector(file, (u16*)fbl);
      if (block_index > 0) {
        file = lfs_find_first_free();
      }
      block_index--;
    }
    free(fbl);
    
  }
  ent->file_first_lba = file;
  lfs_append_table(ent);
  free(ent);
}

void file_buffer_free(File_Buffer* fb) {
  free(fb->items);
}

