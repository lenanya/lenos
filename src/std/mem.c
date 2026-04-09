#include "mem.h"
#include "../vga.h"
#include "string.h"
#include "io.h"

void* __HEAP_BASE = NULL;
void* __HEAP_TOP = NULL;

void memncpy(void* src, void* dest, u32 n) {
	for (u32 i = 0; i < n; ++i) {
		((addr)dest)[i] = ((addr)src)[i];
	}
}

void heap_dump(void) {
	Mem_Header* header = (Mem_Header*)__HEAP_BASE;
	u32 i = 0;
	printf("__HEAP_BASE: %p\n", __HEAP_BASE);
	printf(" __HEAP_TOP: %p\n", __HEAP_TOP);
	do {
		printf("%u at %p\n", i, header);
		printf("     size: %u\n", header->size);
		printf("  is_free: %b\n", header->is_free);
		printf("     next: %p\n", header->next);
		printf("     prev: %p\n", header->prev);
		if (header->owner) printf("    owner: %s\n", header->owner);
		println("");
		++i;
		header = header->next;
	} while (header->next);
	printf("%u at %p\n", i, header);
	printf("     size: %u\n", header->size);
	printf("  is_free: %b\n", header->is_free);
	printf("     next: %p\n", header->next);
	printf("     prev: %p\n\n", header->prev);
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

	largest_start += (32 - largest_start % 32);

	__HEAP_BASE = (void*)(u32)largest_start;
	__HEAP_TOP  = (void*)(u32)(largest_start + largest_size);
}

void* malloc(u32 size) {
	if (size % 32 != 0) {
		size += (32 - size % 32);
	}
	if (__HEAP_BASE == NULL) {
		mem_setup_heap_vars();
		if ((u32)__HEAP_BASE + size > (u32)__HEAP_TOP) {
			eprintln("malloc returning null");
			return NULL;
		}
		u32 alloc_size = sizeof(Mem_Header) + size;
		*((Mem_Header*)__HEAP_BASE) = (Mem_Header) {
			.size = size,
			.is_free = false,
			.next = (Mem_Header*)((addr)__HEAP_BASE + alloc_size),
			.prev = NULL,
		};

		Mem_Header* next = ((Mem_Header*)((addr)__HEAP_BASE + alloc_size));
		next->size = __HEAP_TOP - __HEAP_BASE - alloc_size;
		next->is_free = true;
		next->prev = (Mem_Header*)__HEAP_BASE;
		next->next = NULL;

		return __HEAP_BASE + sizeof(Mem_Header);
	}

	Mem_Header* header = (Mem_Header*)__HEAP_BASE;
	for (;;) {
		if (header->is_free) {
			if (header->size >= size) {
				u32 extra_size = 0;
				if (header->size != size) {
					Mem_Header* org_next = header->next;
					Mem_Header* split = (Mem_Header*)((addr)header + sizeof(Mem_Header) + size);
					if (header->size - size < sizeof(Mem_Header) + 1) {
						header->is_free = false;
						return (addr)header + sizeof(Mem_Header);
					} else {
						*split = (Mem_Header) {
							.size = header->size - sizeof(Mem_Header) - size,
							.is_free = true,
							.next = org_next,
							.prev = header,
						};
						if (org_next) org_next->prev = split;
						header->next = split;
					}

					header->is_free = false;
					header->size = size + extra_size;
					return (addr)header + sizeof(Mem_Header);
				} else {
					header->is_free = false;
					return (addr)header + sizeof(Mem_Header);
				}
			} else {
				if (!header->next) {
					eprintln("malloc returning null");
					return NULL;
				}
				header = header->next; 
			}
 		} else {
			if (!header->next) {
				eprintln("malloc returning null");
				return NULL;
			}
			header = header->next;
		}
	}
}

void free(void* mem) {
	if (!mem) return;
	Mem_Header* header = (Mem_Header*)((addr)mem - sizeof(Mem_Header));
	if (header->is_free) return;
	header->is_free = true;
	header->owner = NULL;
	
	while (header->next != NULL && header->next->is_free) {
		header->size += header->next->size + sizeof(Mem_Header);
		header->next = header->next->next;
		if (header->next) header->next->prev = header;
	}

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
	if (new_size % 32 != 0) {
		new_size += (32 - new_size % 32);
	}
	Mem_Header* header = (Mem_Header*)(mem - sizeof(Mem_Header));
	u32 size = header->size;

	void* new = malloc(new_size);
	give_allocation_name(new, header->owner);
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
	give_allocation_name(new, "memdup");
	memncpy(mem, new, size);
	return new;
}

void give_allocation_name(void* mem, char* name) {
  Mem_Header* header = (Mem_Header*)(mem - sizeof(Mem_Header));
	header->owner = name;
}