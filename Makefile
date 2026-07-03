all: build/lenos.bin

CFLAGS = -std=c11 -m32 -ffreestanding -fno-pie -nostdlib -Wall -Wno-unused-variable -c \
				 -Wno-unused-value -fno-stack-protector -mno-sse -mno-sse2 -mno-mmx   \
				 -mpreferred-stack-boundary=2 -O0 -Wextra -Wno-unused-parameter

OFILES = build/kernel.o build/common.o build/vga.o build/string.o \
				 build/keyboard.o build/mem.o build/ata.o build/file.o build/lfs.o \
				 build/io.o build/std.o build/com1.o build/serial.o

PROGS = build/helloworld.bin build/edit.bin

# finished image

build/lenos.bin: src/lenos.asm build/kernel.bin
	fasm src/lenos.asm build/lenos.bin

# C kernel

build/kernel.bin: $(OFILES) src/kernel.ld
	ld -m elf_i386 -T src/kernel.ld -Ttext 0x9000 $(OFILES) \
	-o build/kernel.bin --oformat binary

build/kernel.o: src/kernel.c src/kernel_std/da.h headers
	cc $(CFLAGS) src/kernel.c -o build/kernel.o

build/common.o: src/common.c src/common.h
	cc $(CFLAGS) src/common.c -o build/common.o

# Kernel drivers basically

build/vga.o: src/kernel/vga.c src/kernel/vga.h
	cc $(CFLAGS) src/kernel/vga.c -o build/vga.o

build/keyboard.o: src/kernel/keyboard.c src/kernel/keyboard.h
	cc $(CFLAGS) src/kernel/keyboard.c -o build/keyboard.o

build/ata.o: src/kernel/ata.c src/kernel/ata.h
	cc $(CFLAGS) src/kernel/ata.c -o build/ata.o

build/lfs.o: src/kernel/lfs.c src/kernel/lfs.h
	cc $(CFLAGS) src/kernel/lfs.c -o build/lfs.o

build/com1.o: src/kernel/com1.c src/kernel/com1.h
	cc $(CFLAGS) src/kernel/com1.c -o build/com1.o

# Kernel standard library

build/string.o: src/kernel_std/string.c src/kernel_std/string.h
	cc $(CFLAGS) src/kernel_std/string.c -o build/string.o

build/mem.o: src/kernel_std/mem.c src/kernel_std/mem.h
	cc $(CFLAGS) src/kernel_std/mem.c -o build/mem.o

build/file.o: src/kernel_std/file.c src/kernel_std/file.h
	cc $(CFLAGS) src/kernel_std/file.c -o build/file.o

build/io.o: src/kernel_std/io.c src/kernel_std/io.h
	cc $(CFLAGS) src/kernel_std/io.c -o build/io.o

build/serial.o: src/kernel_std/serial.c src/kernel_std/serial.h
	cc $(CFLAGS) src/kernel_std/serial.c -o build/serial.o

build/std.o: src/std.c src/std.h
	cc $(CFLAGS) src/std.c -o build/std.o


# Programs

build/helloworld.o: src/progs/helloworld.c
	cc $(CFLAGS) src/progs/helloworld.c -o build/helloworld.o

build/helloworld.bin: build/helloworld.o 
	ld -m elf_i386 -T src/prog.ld -Ttext 0x90000 build/helloworld.o -o build/helloworld.bin --oformat binary


build/edit.o: src/progs/edit.c
	cc $(CFLAGS) src/progs/edit.c -o build/edit.o

build/edit.bin: build/edit.o 
	ld -m elf_i386 -T src/prog.ld -Ttext 0x90000 build/edit.o -o build/edit.bin --oformat binary

# TODO: make FS mountable so i dont need to do this

headers: $(PROGS)
	python binary_to_h.py

clean:
	rm build/*
