CFLAGS = -m32 -ffreestanding -fno-pie -nostdlib -Wall -Wno-unused-variable -c \
				 -Wno-unused-value -fno-stack-protector -mno-sse -mno-sse2 -mno-mmx   \
				 -mpreferred-stack-boundary=2
OFILES = build/kernel.o build/common.o build/vga.o build/string.o \
				 build/keyboard.o build/mem.o build/ata.o build/file.o build/lfs.o

build/lenos.bin: src/lenos.asm build/kernel.bin
	fasm src/lenos.asm build/lenos.bin

build/kernel.bin: $(OFILES) src/kernel.ld
	ld -m elf_i386 -T src/kernel.ld -Ttext 0x9000 $(OFILES) -o build/kernel.bin --oformat binary

build/kernel.o: src/kernel.c src/std/da.h
	cc $(CFLAGS) src/kernel.c -o build/kernel.o

build/common.o: src/common.c src/common.h
	cc $(CFLAGS) src/common.c -o build/common.o

build/vga.o: src/vga.c src/vga.h
	cc $(CFLAGS) src/vga.c -o build/vga.o
	
build/string.o: src/std/string.c src/std/string.h
	cc $(CFLAGS) src/std/string.c -o build/string.o

build/mem.o: src/std/mem.c src/std/mem.h
	cc $(CFLAGS) src/std/mem.c -o build/mem.o

build/keyboard.o: src/keyboard.c src/keyboard.h
	cc $(CFLAGS) src/keyboard.c -o build/keyboard.o

build/ata.o: src/ata.c src/ata.h
	cc $(CFLAGS) src/ata.c -o build/ata.o

build/file.o: src/std/file.c src/std/file.h
	cc $(CFLAGS) src/std/file.c -o build/file.o

build/lfs.o: src/lfs.c src/lfs.h
	cc $(CFLAGS) src/lfs.c -o build/lfs.o

clean:
	rm build/*