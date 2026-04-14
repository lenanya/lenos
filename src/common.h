#ifndef COMMON_H
#define COMMON_H

#define false (bool)0
#define true (bool)1
#define NULL ((void*)0)

typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef char bool;
typedef int i32;
typedef unsigned int u32;
typedef long long i64;
typedef unsigned long long u64;
typedef u8* addr;

typedef u8 LFS_Entry_Flags;
typedef struct LFS_Superblock LFS_Superblock;


#define LFS_MAGIC 0x4c454653

#define LFS_MAX_BLOCKS 2*1024*1024*5

typedef enum LFS_Block_Flags {
  BLOCK_FREE = 0x00,
  BLOCK_USED = 0x01,
} LFS_Block_FLags;

typedef struct LFS_Superblock {
  u32 magic; // magic number for lfs
  u32 sector_count; // how many sectors the data section has
  u32 entry_count;  // how many entries the table has
  u32 data_lba;     // first lba of the raw data
  u32 block_size;   // how large one file block is
  u32 lfs_version;  // lfs version
  u32 next_free;    // next free raw data block
  u32 superblock_lba;
  u8 reserved[512 - (sizeof(u32)*8)];
} __attribute__((packed)) LFS_Superblock;

typedef struct LFS_Table_Entry LFS_Table_Entry;
typedef struct {
  u8 flags;
  u32 next_block_lba;
  char data[512 - sizeof(u32) - sizeof(u8)];
} __attribute__((packed)) LFS_File_Block;

typedef struct LFS_Table_Position {
  u32 lba;
  u32 index;
  bool after_last;
  bool is_last;
} LFS_Table_Position;

typedef struct {
  char* items;
  u32 size;
  u32 capacity;
} String_Buffer;

typedef struct Directory {
  LFS_Table_Entry** items;
  u32 size;
  u32 capacity;
} Directory;

typedef struct LFS_Table_Entry {
  char name[32];         // 32  
  LFS_Entry_Flags flags; // 1
  u32 file_size;        // 4
  bool deleted;          // 1
  bool last;             // 1
  u32 file_first_lba;   // 4
  char reserved[64-4-1-1-4-1-32];
} __attribute__((packed)) LFS_Table_Entry;

typedef struct {
  LFS_File_Block** items;
  u32 size;
  u32 capacity;
  bool exists;
} LFS_File;

typedef struct File_Buffer {
  char* items;
  u32 size;
  u32 capacity;
  bool exists;
} File_Buffer;

typedef enum {
  FILE,
} File_Type;

void outb(u16 port, u8 val);
u8 inb(u16 port);
u16 inw(u16 port);
void outw(u16 port, u16 w);

#endif //COMMON_H