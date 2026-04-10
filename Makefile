all: build/lenos.bin build/prog.bin

CFLAGS = -m32 -ffreestanding -fno-pie -nostdlib -Wall -Wno-unused-variable -c \
				 -Wno-unused-value -fno-stack-protector -mno-sse -mno-sse2 -mno-mmx   \
				 -mpreferred-stack-boundary=2
OFILES = build/kernel.o build/common.o build/vga.o build/string.o \
				 build/keyboard.o build/mem.o build/ata.o build/file.o build/lfs.o \
				 build/io.o build/std.o

build/lenos.bin: src/lenos.asm build/kernel.bin
	fasm src/lenos.asm build/lenos.bin

build/prog.bin: build/prog.o 
	ld -m elf_i386 -T src/prog.ld -Ttext 0x90000 build/prog.o -o build/prog.bin --oformat binary

build/kernel.bin: $(OFILES) src/kernel.ld src/program_content.h
	ld -m elf_i386 -T src/kernel.ld -Ttext 0x9000 $(OFILES) -o build/kernel.bin --oformat binary

src/program_content.h: build/prog.bin
	python binary_to_h.py

build/kernel.o: src/kernel.c src/std/da.h src/program_content.h
	cc $(CFLAGS) src/kernel.c -o build/kernel.o

build/common.o: src/common.c src/common.h
	cc $(CFLAGS) src/common.c -o build/common.o

build/vga.o: src/kernel/vga.c src/kernel/vga.h
	cc $(CFLAGS) src/kernel/vga.c -o build/vga.o
	
build/string.o: src/std/string.c src/std/string.h
	cc $(CFLAGS) src/std/string.c -o build/string.o

build/mem.o: src/std/mem.c src/std/mem.h
	cc $(CFLAGS) src/std/mem.c -o build/mem.o

build/keyboard.o: src/kernel/keyboard.c src/kernel/keyboard.h
	cc $(CFLAGS) src/kernel/keyboard.c -o build/keyboard.o

build/ata.o: src/kernel/ata.c src/kernel/ata.h
	cc $(CFLAGS) src/kernel/ata.c -o build/ata.o

build/file.o: src/std/file.c src/std/file.h
	cc $(CFLAGS) src/std/file.c -o build/file.o

build/io.o: src/std/io.c src/std/io.h
	cc $(CFLAGS) src/std/io.c -o build/io.o

build/lfs.o: src/kernel/lfs.c src/kernel/lfs.h
	cc $(CFLAGS) src/kernel/lfs.c -o build/lfs.o

build/std.o: src/std.c src/std.h
	cc $(CFLAGS) src/std.c -o build/std.o

build/prog.o: src/prog.c
	cc $(CFLAGS) src/prog.c -o build/prog.o

clean:
	rm build/*