#ifndef FILE_H
#define FILE_H

#include "../common.h"

void read_entire_file(File_Buffer* fb, char* filename);
void file_buffer_free(File_Buffer* fb);
void write_entire_file(File_Buffer* fb, char* filename);
void delete_file(char* filename);

#endif // FILE_H