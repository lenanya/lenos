#ifndef MEM_H
#define MEM_H
#include "../common.h"

#define NULL ((void*)0)
#define MEM_MIN_ALLOC 16

static void* __HEAP_BASE = NULL;
static void* __HEAP_TOP  = NULL;

typedef struct Mem_Header Mem_Header;

typedef struct Mem_Header {
  u32 size;
  bool is_free;
  Mem_Header* next;
  Mem_Header* prev;
  Mem_Header* start;
} __attribute__((packed)) Mem_Header;

typedef struct {
  u64 base;
  u64 length;
  u32 type;
  u32 acpi;
} __attribute__((packed)) SMAP_Entry;

void memncpy(void* src, void* dest, u32 n);
void* malloc(u32 size);
void free(void* mem);
void* realloc(void* mem, u32 new_size);
void* calloc(u32 size, char b);
void* memdup(void* mem, u32 size);

#endif // MEM_H