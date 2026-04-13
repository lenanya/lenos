#ifndef MEM_H
#define MEM_H
#include "../common.h"

#define NULL ((void*)0)
#define MEM_MIN_ALLOC 16

extern void* __HEAP_BASE;
extern void* __HEAP_TOP;

typedef struct Mem_Header Mem_Header;

typedef struct Mem_Header {
  u32 size;
  bool is_free;
  Mem_Header* next;
  Mem_Header* prev;
  char* owner;
} __attribute__((packed)) Mem_Header;

typedef struct {
  u64 base;
  u64 length;
  u32 type;
  u32 acpi;
} __attribute__((packed)) SMAP_Entry;

typedef struct Mem_Allocs {
  addr* items;
  u32 size;
  u32 capacity;
} Mem_Allocs;

void memncpy(void* src, void* dest, u32 n);
void* malloc(u32 size);
void free(void* mem);
void* realloc(void* mem, u32 new_size);
void* calloc(u32 size, char b);
void* memdup(void* mem, u32 size);
void heap_dump(void);
void give_allocation_name(void* mem, char* name);
void* user_malloc(u32 size);
void free_user_allocations();

#endif // MEM_H