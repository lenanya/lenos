#ifndef LFS_H
#define LFS_H
#include "common.h"

typedef u8 LFS_Entry_Flags;
#define LFS_MAGIC 0x4c454653
#define LFS_MAX_BLOCKS 2*1024*1024*5

typedef enum LFS_Block_Flags {
  BLOCK_FREE = 0x00,
  BLOCK_USED = 0x01,
} LFS_Block_FLags;

typedef struct {
  u32 magic; // magic number for lfs
  u32 sector_count; // how many sectors the data section has
  u32 entry_count;  // how many entries the table has
  u32 data_lba;     // first lba of the raw data
  u32 block_size;   // how large one file block is
  u32 lfs_version;  // lfs version
  u32 next_free;    // next free raw data block
  u8 reserved[512 - (sizeof(u32)*7)];
} __attribute__((packed)) LFS_Superblock;

typedef struct LFS_Table_Entry LFS_Table_Entry;

typedef struct LFS_Table_Entry {
  char name[32];         // 32  
  LFS_Entry_Flags flags; // 1
  u32 file_size;        // 4
  bool deleted;          // 1
  bool last;             // 1
  u32 file_first_lba;   // 4
  char reserved[64 - 35];
} __attribute__((packed)) LFS_Table_Entry;

typedef struct {
  u8 flags;
  u32 next_block_lba;
  char data[512 - sizeof(u32) - sizeof(u8)];
} __attribute__((packed)) LFS_File_Block;

typedef struct {
  LFS_File_Block** items;
  u32 size;
  u32 capacity;
  bool exists;
} __attribute__((packed)) LFS_File;

typedef struct Directory {
  LFS_Table_Entry** items;
  u32 size;
  u32 capacity;
} Directory;

LFS_Table_Entry* lfs_find_file(char* name);
void lfs_read_directory(Directory* dir);
u32 lfs_find_first_free_table_block();
u32 lfs_find_first_free();
LFS_Superblock* lfs_get_superblock(void);
void lfs_append_table(LFS_Table_Entry* te);
u32 lfs_find_second_free();
void lfs_write_superblock(LFS_Superblock* sb);

#endif // LFS_H