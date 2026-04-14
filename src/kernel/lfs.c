#include "lfs.h"
#include "../kernel_std/da.h"
#include "ata.h"
#include "../kernel_std/string.h"
#include "vga.h"
#include "../kernel_std/file.h"
#include "../kernel_std/io.h"

LFS_Superblock* sb = NULL;


void print_table_entry(LFS_Table_Entry* te) {
  //  char name[32];         // 32  
  //  LFS_Entry_Flags flags; // 1
  //  u32 file_size;        // 4
  //  bool deleted;          // 1
  //  bool last;             // 1
  //  u32 file_first_lba;   // 4
  printf("          name: %s\n", te->name);
  printf("         flags: %c\n", te->flags);
  printf("     file_size: %u\n", te->file_size);
  printf("       deleted: %b\n", te->deleted);
  printf("          last: %b\n", te->last);
  printf("file_first_lba: %u\n\n", te->file_first_lba);
}

LFS_Superblock* lfs_get_superblock(void) {
  u16* block_buf = (u16*)malloc(512);
  give_allocation_name(block_buf, "lfs_get_superblock");
  if (!sb) {
    sb = malloc(sizeof(LFS_Superblock));
    give_allocation_name(sb, "sb");
  }
  u32 block = 1;
  do {
    for (u32 i = 0; i < 256; ++i) block_buf[i] = 0;
    ata_read_sectors(block, 1, block_buf);
    memncpy(block_buf, sb, sizeof(LFS_Superblock));
    if (sb->magic == LFS_MAGIC) {
      free(block_buf);
      sb->superblock_lba = block;
      lfs_write_superblock(sb);
      return sb;
    }
    block++;
  } while (block < LFS_MAX_BLOCKS);
  free(block_buf);
  return NULL;
}

void lfs_write_superblock(LFS_Superblock* sb_local) {
  ata_write_sector(sb->superblock_lba, (u16*)sb_local);
}

void lfs_append_table(LFS_Table_Entry* te) {
  LFS_Superblock* sb_local = lfs_get_superblock();
  LFS_Table_Position p = lfs_find_first_free_table_block();
  u32 index = p.index;

  u16* block_buffer = (u16*)malloc(512);
  give_allocation_name(block_buffer, "lfs_append_table");
  ata_read_sectors(sb_local->superblock_lba + p.lba, 1, block_buffer);
  if (p.after_last) {
    if (p.index != 0) {
      (((LFS_Table_Entry*)block_buffer)+(index-1))->last = false;
    } else {
      u16* bb2 = malloc(512);
      ata_read_sectors(sb_local->superblock_lba + p.lba - 1, 1, bb2);
      (((LFS_Table_Entry*)block_buffer)+(7))->last = false;
      ata_write_sector(sb_local->superblock_lba + p.lba - 1, bb2);
      free(bb2);
    }
  }
  if (p.is_last) {
    te->last = true;
  }
  memncpy(te, ((LFS_Table_Entry*)block_buffer)+index, sizeof(LFS_Table_Entry));
  ata_write_sector(sb_local->superblock_lba + p.lba, block_buffer);
  free(block_buffer);
}

LFS_Table_Entry* lfs_find_file(char* name) {
  if (*name == 0) return NULL;
  LFS_Superblock* sb_local = lfs_get_superblock();
  if (sb_local == NULL) return NULL;
  if (sb_local->entry_count < 1) return NULL;
  LFS_Table_Entry* te = NULL;
  u32 block = 1;
  u16* volatile block_buf = (u16*)malloc(512);
  give_allocation_name(block_buf, "lfs_find_file");
  while (block < LFS_MAX_BLOCKS) {
    if (sb_local->superblock_lba + block > LFS_MAX_BLOCKS) return NULL;
    ata_read_sectors(sb_local->superblock_lba + block, 1, block_buf);
    te = (LFS_Table_Entry*)block_buf;
    while ((u32)te < (u32)block_buf+512) {
      if (strcmp(te->name, name) == true) {
        LFS_Table_Entry* ret = (LFS_Table_Entry*)memdup(te, sizeof(LFS_Table_Entry));
        free(block_buf);
        return ret;
      }
      if (te->last) {
        free(block_buf);
        return NULL;
      }
      te++;
    }
    block++;
  };
  free(block_buf);
  return NULL;
}

u32 lfs_find_first_free() {
  LFS_Superblock* sb_local = lfs_get_superblock();
  if (sb_local == NULL) while(1);
  u16* volatile block_buf = (u16*)malloc(512);
  give_allocation_name(block_buf, "lfs_find_first_free");
  LFS_File_Block* fb = NULL;
  u32 lba = sb_local->data_lba;
  while (lba < LFS_MAX_BLOCKS) {
    ata_read_sectors(lba, 1, block_buf);
    fb = (LFS_File_Block*)block_buf;
    
    if (!fb->flags) {
      free(block_buf);
      return lba;
    }
    lba++;
  }
  free(block_buf);
  while(1);
}

u32 lfs_find_second_free() {
  LFS_Superblock* sb_local = lfs_get_superblock();
  u32 first = lfs_find_first_free();
  u16* volatile block_buf = (u16*)malloc(512);
  give_allocation_name(block_buf, "lfs_find_second_free");
  LFS_File_Block* te = NULL;
  u32 lba = first + 1;
  while (lba < LFS_MAX_BLOCKS) {
    ata_read_sectors(lba, 1, block_buf);
    te = (LFS_File_Block*)block_buf;
    while ((u32)te < (u32)block_buf+512) {
      if (!te->flags) {
        free(block_buf);
        return lba;
      }
      te++;
    }
    lba++;
  }
  free(block_buf);
  return 0;
}

LFS_Table_Position lfs_find_first_free_table_block() {
  LFS_Superblock* sb_local = lfs_get_superblock();
  u8* block_buf = (u8*)malloc(512);
  u32 block = 1;

  while (block < LFS_MAX_BLOCKS) {
    u32 current_lba = sb_local->superblock_lba + block;
    ata_read_sectors(current_lba, 1, (u16*)block_buf);
    LFS_Table_Entry* entries = (LFS_Table_Entry*)block_buf;
    
    for (u32 sector = 0; sector < 8; ++sector) {
      if (entries[sector].deleted) {
        LFS_Table_Position p = (LFS_Table_Position) {
          .index = sector, 
          .lba = block, 
          .after_last = false,
          .is_last = false
        };
        if (entries[sector].last) p.is_last = true;
        return p;
      }
      if (entries[sector].last) {
        if (sector == 7) {
          LFS_Table_Position p = (LFS_Table_Position) {
            .index = 0, 
            .lba = block+1, 
            .after_last = true,
            .is_last = true
          };
          return p;
        } else {
          LFS_Table_Position p = (LFS_Table_Position) {
            .index = sector+1, 
            .lba = block, 
            .after_last = true,
            .is_last = true
          };
          return p;
        }
      }
    }
    block++;
  }
  free(block_buf);
  return (LFS_Table_Position){.index = 0, .lba = 0};
}

void lfs_read_directory(Directory* dir) {
  LFS_Superblock* sb_local = lfs_get_superblock();
  if (sb_local == NULL) return;
  if (sb_local->entry_count < 1) return;
  dir->size = 0;
  LFS_Table_Entry* te = NULL;
  u32 block = 1;
  u16* block_buf = (u16*)calloc(512, 0);
  give_allocation_name(block_buf, "lfs_read_directory");
  te = (LFS_Table_Entry*)block_buf;
  do {
    if (sb_local->superblock_lba + block > LFS_MAX_BLOCKS) break;
    ata_read_sectors(sb_local->superblock_lba + block, 1, block_buf);
    te = (LFS_Table_Entry*)block_buf;
    while ((u32)te < (u32)block_buf+512) {
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
  } while (!te->last);
  free(block_buf);
} 

void lfs_delete_file(char* filename) {
  LFS_Table_Entry* te = lfs_find_file(filename);
  if (!te) {
    free(te);
    return;
  }
  u32 lba = te->file_first_lba;
  LFS_Superblock* sb_local = lfs_get_superblock();
  if (sb_local == NULL) return;
  if (sb_local->entry_count < 1) return;
  u32 block = 1;
  u16* block_buf = (u16*)calloc(512, 0);
  give_allocation_name(block_buf, "lfs_read_directory");
  LFS_Table_Entry* te2 = (LFS_Table_Entry*)block_buf;
  do {
    if (sb_local->superblock_lba + block > LFS_MAX_BLOCKS) break;
    ata_read_sectors(sb_local->superblock_lba + block, 1, block_buf);
    for (u32 i = 0; i < 8; ++i) {
      te2 = ((LFS_Table_Entry*)block_buf)+i;
      if (strcmp(filename, te2->name) == true) {
        te2->deleted = true;
        for (u32 j = 0; j < 32; ++j) {
          te2->name[j] = 0;
        }
        ata_write_sector(sb_local->superblock_lba + block, block_buf);
        goto done;
      }
    }
    block++;
  } while (!te2->last);
  done:
  while (lba != 0) {
    u32 lba2 = lba;
    ata_read_sectors(lba, 1, block_buf);
    lba = ((LFS_File_Block*)block_buf)->next_block_lba;
    for (u32 i = 0; i < 256; ++i) {
      block_buf[i] = 0;
    } 
    ata_write_sector(lba2, block_buf);
  }
  free(block_buf);
}

void lfs_file_block_free(LFS_File_Block* b) {
  free(b->data);
}

void lfs_file_free(LFS_File* f) {
  free(f->items);
}