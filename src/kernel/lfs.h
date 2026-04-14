#ifndef LFS_H
#define LFS_H
#include "../common.h"

LFS_Table_Entry* lfs_find_file(char* name);
void lfs_read_directory(Directory* dir);
LFS_Table_Position lfs_find_first_free_table_block();
u32 lfs_find_first_free();
LFS_Superblock* lfs_get_superblock(void);
void lfs_append_table(LFS_Table_Entry* te);
u32 lfs_find_second_free();
void lfs_write_superblock(LFS_Superblock* sb);
void print_table_entry(LFS_Table_Entry* te);
void lfs_delete_file(char* filename);

#endif // LFS_H