#include "mem.h"

void memncpy(void* src, void* dest, int n) {
	for (int i = 0; i < n; ++i) {
	((char*)dest)[i] = ((char*)src)[i];
	}
}

void mem_setup_heap_vars(void) {
	SMAP_Entry* smap = (SMAP_Entry*)0x3000;
	ushort entry_count = *(ushort*)0x2ff0;
	ulong largest_start = 0;
	ulong largest_size  = 0;

	for (uint i = 0; i < entry_count; ++i) {
		if (smap[i].type == 1) {
			ulong start = smap[i].base;
			if (start > 0xFFFFFFFF) continue;
			if (start < 0x90000) continue;
			ulong end   = smap[i].base + smap[i].length;
			if (end > 0xFFFFFFFF) end = 0xFFFFFFFF;
			ulong size  = end - start;
			if (size > largest_size) {
				largest_start = start;
				largest_size = size;
			}
		}
	}
	ulong largest_end = largest_start + largest_size;
	if (0xFFFFFFFF - largest_end > largest_size) {
		__HEAP_BASE = (void*)(uint)largest_end;
		__HEAP_TOP = (void*)0xFFFFFFFF;
		return;
	}

	__HEAP_BASE = (void*)(uint)largest_start;
	__HEAP_TOP  = (void*)(uint)largest_start + largest_size;
}

void* malloc(uint size) {
	if (size % 16 != 0) {
		size += (16 - size % 16);
	}
	if (__HEAP_BASE == NULL) {
		mem_setup_heap_vars();
		if ((uint)__HEAP_BASE + size > (uint)__HEAP_TOP) return NULL;
		uint alloc_size = sizeof(Mem_Header) + size;
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
				Mem_Header* next = header->start + size;
				uint extra_size = 0;
				if (!(header->size == size)) {
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
				header->next = next;
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

	while (header->prev != NULL && header->prev->is_free) {
		header->prev->size += header->size + sizeof(Mem_Header);
		header->prev->next = header->next;
		if (header->next) {
			header->next->prev = header->prev;
		}
		header = header->prev;
	}
}

void* realloc(void* mem, uint new_size) {
	if (!mem) return malloc(new_size);
	Mem_Header* header = (Mem_Header*)(mem - sizeof(Mem_Header));
	uint size = header->size;
	void* new = malloc(new_size);
	memncpy(mem, new, new_size);
	free(mem);
	return new;
}

void* calloc(uint size, char b) {
	void* mem = malloc(size);
	for (int i = 0; i < size; ++i) {
		((char*)mem)[i] = b;
	}
	return mem;
}

void* memdup(void* mem, uint size) {
	void* new = malloc(size);
	memncpy(mem, new, size);
	return new;
}