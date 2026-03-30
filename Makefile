CFLAGS = -m32 -ffreestanding -fno-pie -nostdlib -c
OFILES = build/kernel.o build/common.o build/vga.o build/string.o

build/lenos.bin: src/lenos.asm build/kernel.bin
	fasm src/lenos.asm build/lenos.bin

build/kernel.bin: $(OFILES) src/kernel.ld
	ld -m elf_i386 -T src/kernel.ld -Ttext 0x9000 $(OFILES) -o build/kernel.bin --oformat binary

build/kernel.o: src/kernel.c 
	cc $(CFLAGS) src/kernel.c -o build/kernel.o

build/common.o: src/common.c src/common.h
	cc $(CFLAGS) src/common.c -o build/common.o

build/vga.o: src/vga.c src/vga.h
	cc $(CFLAGS) src/vga.c -o build/vga.o
	
build/string.o: src/std/string.c src/std/string.h
	cc $(CFLAGS) src/std/string.c -o build/string.o