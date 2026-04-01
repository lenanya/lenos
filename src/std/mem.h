#ifndef MEM_H
#define MEM_H
#include "../common.h"

#define NULL ((void*)0)
#define MEM_MIN_ALLOC 16

static void* __HEAP_BASE = NULL;
static void* __HEAP_TOP  = NULL;

typedef struct Mem_Header Mem_Header;

typedef struct Mem_Header {
  uint size;
  bool is_free;
  Mem_Header* next;
  Mem_Header* prev;
  Mem_Header* start;
} __attribute__((packed)) Mem_Header;

typedef struct {
  ulong base;
  ulong length;
  uint type;
  uint acpi;
} __attribute__((packed)) SMAP_Entry;

void memncpy(void* src, void* dest, int n);
void* malloc(uint size);
void free(void* mem);
void* realloc(void* mem, uint new_size);
void* calloc(uint size, char b);

#endif // MEM_H