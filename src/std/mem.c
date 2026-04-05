#include "mem.h"
#include "../vga.h"
#include "string.h"
#include "io.h"

void memncpy(void* src, void* dest, u32 n) {
	for (u32 i = 0; i < n; ++i) {
		((char*)dest)[i] = ((char*)src)[i];
	}
}

void mem_setup_heap_vars(void) {
	SMAP_Entry* smap = (SMAP_Entry*)0x3000;
	u16 entry_count = *(u16*)0x2ff0;
	u64 largest_start = 0;
	u64 largest_size  = 0;

	for (u32 i = 0; i < entry_count; ++i) {
		if (smap[i].type == 1) {
			u64 start = smap[i].base;
			if (start > 0xFFFFFFFF) continue;
			if (start < 0x90000) continue;
			u64 end   = smap[i].base + smap[i].length;
			if (end > 0xFFFFFFFF) end = 0xFFFFFFFF;
			u64 size  = end - start;
			if (size > largest_size) {
				largest_start = start;
				largest_size = size;
			}
		}
	}
	u64 largest_end = largest_start + largest_size;
	if (0xFFFFFFFF - largest_end > largest_size) {
		__HEAP_BASE = (void*)(u32)largest_end;
		__HEAP_TOP = (void*)0xFFFFFFFF;
		return;
	}

	__HEAP_BASE = (void*)(u32)largest_start;
	__HEAP_TOP  = (void*)(u32)largest_start + largest_size;
}

void* malloc(u32 size) {
	//eprintln("malloc called.");
	if (size % 16 != 0) {
		size += (16 - size % 16);
	}
	if (__HEAP_BASE == NULL) {
		mem_setup_heap_vars();
		if ((u32)__HEAP_BASE + size > (u32)__HEAP_TOP) return NULL;
		u32 alloc_size = sizeof(Mem_Header) + size;
		*((Mem_Header*)__HEAP_BASE) = (Mem_Header) {
			.size = size,
			.is_free = false,
			.next = __HEAP_BASE+alloc_size,
			.prev = NULL,
			.start = __HEAP_BASE + sizeof(Mem_Header)
		};

		Mem_Header* next = ((Mem_Header*)(__HEAP_BASE + alloc_size));
		next->size = __HEAP_TOP - __HEAP_BASE - alloc_size;
		next->is_free = true;
		next->prev = __HEAP_BASE;
		next->next = NULL;
		next->start = next + 1;

		return __HEAP_BASE + sizeof(Mem_Header);
	}

	Mem_Header* header = (Mem_Header*)__HEAP_BASE;
	for (;;) {
		if (header->is_free) {
			if (header->size >= size) {
				u32 extra_size = 0;
				if (!(header->size == size)) {
					Mem_Header* next = header->start + size;
					if (header->size - size < sizeof(Mem_Header) + 1) {
						extra_size = header->size - size;
					} else {
						*next = (Mem_Header) {
							.size = header->size - sizeof(Mem_Header) - size,
							.is_free = true,
							.next = header->next,
							.prev = header,
							.start = next + 1
						};
					}
					header->is_free = false;
					header->size = size + extra_size;
					header->next = next;
					return header->start;
				}
				header->is_free = false;
				return header->start; 
			} else {
				if (!header->next) return NULL;
				header = header->next; 
			}
 		} else {
			if (!header->next) return NULL;
			header = header->next;
		}
	}
}

void free(void* mem) {
	if (!mem) return;
	Mem_Header* header = (Mem_Header*)(mem - sizeof(Mem_Header));
	header->is_free = true;
	if (header->next == NULL) return;
	while (header->next != NULL && header->next->is_free) {
		header->size += header->next->size + sizeof(Mem_Header);
		header->next = header->next->next;
	}

	if (header->next) header->next->prev = header;

	while (header->prev != NULL && header->prev->is_free) {
		header->prev->size += header->size + sizeof(Mem_Header);
		header->prev->next = header->next;
		if (header->next) {
			header->next->prev = header->prev;
		}
		header = header->prev;
	}
}

void* realloc(void* mem, u32 new_size) {
	if (!mem) return malloc(new_size);
	Mem_Header* header = (Mem_Header*)(mem - sizeof(Mem_Header));
	u32 size = header->size;
	void* new = malloc(new_size);
	memncpy(mem, new, size);
	free(mem);
	return new;
}

void* calloc(u32 size, char b) {
	void* mem = malloc(size);
	for (int i = 0; i < size; ++i) {
		((char*)mem)[i] = b;
	}
	return mem;
}

void* memdup(void* mem, u32 size) {
	void* new = malloc(size);
	memncpy(mem, new, size);
	return new;
}