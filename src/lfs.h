#ifndef LFS_H
#define LFS_H
#include "common.h"

typedef uchar LFS_Entry_Flags;
#define LFS_MAGIC 0x4c454653
#define LFS_MAX_BLOCKS 8388608

typedef struct {
  uint magic; // magic number for lfs
  uint sector_count; // how many sectors the data section has
  uint entry_count;  // how many entries the table has
  uint data_lba;     // first lba of the raw data
  uint block_size;   // how large one file block is
  uint lfs_version;  // lfs version
  uint next_free;    // next free raw data block
  uchar reserved[512 - (sizeof(uint)*7)];
} __attribute__((packed)) LFS_Superblock;

typedef struct LFS_Table_Entry LFS_Table_Entry;

typedef struct LFS_Table_Entry {
  char name[32];         // 32  
  LFS_Entry_Flags flags; // 1
  uint file_size;        // 4
  bool deleted;          // 1
  bool last;             // 1
  uint file_first_lba;   // 4
  char reserved[64 - 35];
} __attribute__((packed)) LFS_Table_Entry;

typedef struct {
  uint next_block_lba;
  char data[512 - sizeof(uint)];
} __attribute__((packed)) LFS_File_Block;

typedef struct {
  LFS_File_Block** items;
  uint size;
  uint capacity;
  bool exists;
} __attribute__((packed)) LFS_File;

typedef struct Directory {
  LFS_Table_Entry** items;
  uint size;
  uint capacity;
} Directory;

void lfs_get_superblock(void);
LFS_Table_Entry* lfs_find_file(char* name);
void lfs_read_directory(Directory* dir);

#endif // LFS_H