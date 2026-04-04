#include "lfs.h"
#include "std/da.h"
#include "ata.h"
#include "std/string.h"
#include "vga.h"
#include "std/file.h"

static u32 __lfs_superblock_lba = 0;

LFS_Superblock* lfs_get_superblock(void) {
  u16* block_buf = (u16*)malloc(512);
  u32 block = 1;
  LFS_Superblock* sb = NULL;
  do {
    ata_read_sectors(block, 1, block_buf);
    sb = (LFS_Superblock*)block_buf;
    if (sb->magic == LFS_MAGIC) {
      __lfs_superblock_lba = block;
      return sb;
    }
    block++;
  } while (block < LFS_MAX_BLOCKS);
  return NULL;
}

LFS_Superblock* lfs_get_superblock_ptr() {
  return lfs_get_superblock();
}

void lfs_write_superblock(LFS_Superblock* sb) {
  ata_write_sector(__lfs_superblock_lba, (u16*)sb);
}

void lfs_append_table(LFS_Table_Entry* te) {
  LFS_Superblock* sb = lfs_get_superblock();
  u32 lba = lfs_find_first_free_table_block();
  u32 index = sb->entry_count % 8;
  u16* block_buffer = (u16*)malloc(512);
  ata_read_sectors(lba, 1, block_buffer);
  memncpy(te, ((LFS_Table_Entry*)block_buffer)+index, sizeof(LFS_Table_Entry));
  if (index > 0) {
    ((LFS_Table_Entry*)block_buffer)[index-1].last = 0;
    ata_write_sector(lba, block_buffer);
  } else {
    ata_write_sector(lba, block_buffer);
    ata_read_sectors(lba-1, 1, block_buffer);
    ((LFS_Table_Entry*)block_buffer)[7].last = 0;
    ata_write_sector(lba, block_buffer);
  }     
}

LFS_Table_Entry* lfs_find_file(char* name) {
  if (*name == 0) return NULL;
  LFS_Superblock* sb = lfs_get_superblock();
  if (sb == NULL) return NULL;
  if (sb->entry_count < 1) return NULL;
  LFS_Table_Entry* te = NULL;
  u32 block = 1;
  u16* volatile block_buf = (u16*)malloc(512);
  while (block < LFS_MAX_BLOCKS) {
    if (__lfs_superblock_lba + block > LFS_MAX_BLOCKS) return NULL;
    ata_read_sectors(__lfs_superblock_lba + block, 1, block_buf);
    te = (LFS_Table_Entry*)block_buf;
    while ((u32)te < (u32)(sb + 2 * 512)) {
      if (strcmp(te->name, name) == true) return te;
      if (te->last) return NULL;
      te++;
    }
    block++;
  };
  free(block_buf);
  return NULL;
}

u32 lfs_find_first_free() {
  LFS_Superblock* sb = lfs_get_superblock();
  if (sb == NULL) return 0;
  u16* volatile block_buf = (u16*)malloc(512);
  LFS_File_Block* te = NULL;
  u32 lba = sb->data_lba;
  while (lba < LFS_MAX_BLOCKS) {
    ata_read_sectors(lba, 1, block_buf);
    te = (LFS_File_Block*)block_buf;
    if (!te->flags) return lba;
    lba++;
  }
  free(block_buf);
  return 0;
}

u32 lfs_find_second_free() {
  LFS_Superblock* sb = lfs_get_superblock();
  u32 first = lfs_find_first_free();
  u16* volatile block_buf = (u16*)malloc(512);
  LFS_File_Block* te = NULL;
  u32 lba = first + 1;
  while (lba < LFS_MAX_BLOCKS) {
    ata_read_sectors(lba, 1, block_buf);
    te = (LFS_File_Block*)block_buf;
    while ((u32)te < (u32)(sb + 2 * 512)) {
      if (!te->flags) return lba;
      te++;
    }
    lba++;
  }
  free(block_buf);
  return 0;
}

u32 lfs_find_first_free_table_block() {
  LFS_Superblock* sb = lfs_get_superblock();
  if (sb == NULL) return 0;
  u16* volatile block_buf = (u16*)malloc(512);
  LFS_Table_Entry* te = NULL;
  u16 block = 1;
  while (block < LFS_MAX_BLOCKS) {
    if (__lfs_superblock_lba + block > LFS_MAX_BLOCKS) return 0;
    ata_read_sectors(__lfs_superblock_lba + block, 1, block_buf);
    te = (LFS_Table_Entry*)block_buf;
    u32 section = 0;
    while ((u32)te < (u32)(sb + 2 * 512)) {
      if (te->last) {
        if (section == 7) return __lfs_superblock_lba + block + 1;
        return (__lfs_superblock_lba + block);
      }
      te++;
      section++;
    }
    block++;
  };
}

void lfs_read_directory(Directory* dir) {
  LFS_Superblock* sb = lfs_get_superblock();
  if (sb == NULL) return;
  if (sb->entry_count < 1) return;
  dir->size = 0;
  LFS_Table_Entry* te = NULL;
  u32 block = 1;
  u16* volatile block_buf = (u16*)malloc(512);
  while (!te->last) {
    if (__lfs_superblock_lba + block > LFS_MAX_BLOCKS) return;
    ata_read_sectors(__lfs_superblock_lba + block, 1, block_buf);
    te = (LFS_Table_Entry*)block_buf;
    vga_print_string(te->name);
    vga_print_char(10);
    vga_flip_buffer();
    while ((u32)te < (u32)(sb + 2 * 512)) {
      if (te->deleted) {
        if (te->last) {
          break;
        } else te++;
      } else {
        da_append(dir, (LFS_Table_Entry*)memdup(te, sizeof(LFS_Table_Entry)));
        if (te->last) break;
        te++;
      }
    }
    block++;
  };
  //free(block_buf);
} 

void lfs_create_file(char* name, File_Buffer* fb) {
  while (1);
}

void lfs_file_block_free(LFS_File_Block* b) {
  free(b->data);
}

void lfs_file_free(LFS_File* f) {
  free(f->items);
}