#include "lfs.h"
#include "std/da.h"
#include "ata.h"
#include "std/string.h"
#include "vga.h"
#include "std/file.h"

static LFS_Superblock* __lfs_superblock = NULL;
static uint __lfs_superblock_lba = 0;

void lfs_get_superblock(void) {
  ushort* block_buf = (ushort*)malloc(512);
  uint block = 1;
  LFS_Superblock* sb = NULL;
  do {
    ata_read_sectors(block, 1, block_buf);
    sb = (LFS_Superblock*)block_buf;
    if (sb->magic == LFS_MAGIC) {
      __lfs_superblock = sb;
      __lfs_superblock_lba = block;
      return;
    }
    block++;
  } while (block < LFS_MAX_BLOCKS);
}

LFS_Table_Entry* lfs_find_file(char* name) {
  if (*name == 0) return NULL;
  if (__lfs_superblock == NULL) lfs_get_superblock();
  if (__lfs_superblock == NULL) return NULL;
  if (__lfs_superblock->entry_count < 1) return NULL;
  LFS_Table_Entry* te = NULL;
  uint block = 1;
  ushort* volatile block_buf = (ushort*)malloc(512);
  while (block < 5) {
    if (__lfs_superblock_lba + block > LFS_MAX_BLOCKS) return NULL;
    ata_read_sectors(__lfs_superblock_lba + block, 1, block_buf);
    te = (LFS_Table_Entry*)block_buf;
    while ((uint)te < __lfs_superblock + 2 * 512) {
      if (strcmp(te->name, name) == true) return te;
      if (te->last) return NULL;
      te++;
    }
    block++;
  };
  free(block_buf);
  return NULL;
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