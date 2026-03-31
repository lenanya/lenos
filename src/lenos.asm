format binary
use16
org 0x7c00

stage1:
	; setup stack
	mov ax, 0
	mov ss, ax
	mov sp, 0x7c00 ; before stage 1
	mov bp, sp

	mov [driveindex], dl ;save drive index
    
	call clear

  ; show the loading message of stage 1
	mov bp, loadingmsg
	mov cx, loadingmsglen
	mov bl, 0x03 ; cyan
	mov dl, 32
	call showmessage 
	
	; check if bios supports extended read
	mov dl, [driveindex]
	clc 
	mov ah, 0x41 
	mov bx, 0x55aa
	int 0x13
	jc noextread
	cmp bx, 0xaa55
	jne noextread

	mov ax, 0
	mov ds, ax ; segment with DAP
	mov ah, 0x42 ; extended read
	mov dl, [driveindex]
	mov si, dap ; DAP structure
	int 0x13 ; do the read to load stage2 into RAM
	jc extreadfail

	jmp 0x0000:init_stage2 ; move to stage 2

noextread:
	call clear    

	; print error message
	mov bp, noextreadmsg
	mov cx, noextreadmsglen
	mov bl, 0x84 ; red
	mov dl, 20
	call showmessage

	jmp $ ; infinite loop, "crash"

extreadfail:
	call clear

	mov bp, extreadfailmsg
	mov cx, extreadfailmsglen
	mov bl, 0x84 ; red
	mov dl, 15
	call showmessage

	jmp $

; bp -> address of string
; cx -> length
; bl -> colour
; dl -> column to start in
showmessage:
	mov ax, 0
	mov es, ax ; set segment
	mov ah, 0x13 ; print string
	mov al, 0x01 ; mode, move after printing
	mov bh, 0    ; page number
	mov dh, 12    ; row to start in
	int 0x10
	ret

clear:
	mov ah, 0x06 ; scroll up
	mov al, 0    ; amount of lines, 0 to clear
	mov bh, 0x03 ; colour, cyan on dark grey
	mov ch, 0x00 ; row, top left corner
	mov cl, 0x00 ; col, top left corner
	mov dh, 24   ; row, bottom right corner
	mov dl, 79   ; col, bottomr right corner
	int 0x10     
	mov ah, 0x02 ; move cursor
	mov bh, 0    ; page number (0 -> default video)
	mov dx, 0 ; dh/dl -> row/column, set both to 0
	int 0x10
	ret

driveindex db 0

loadingmsg db "Stage 1 of BIOS"
loadingmsglen = $-loadingmsg

noextreadmsg db "ERROR: BIOS doesnt support Extended Read"
noextreadmsglen = $-noextreadmsg

extreadfailmsg db "ERROR: Bootloader couldn't read Stage 2 from Disk"
extreadfailmsglen = $-extreadfailmsg

align 4
dap:
	db 0x10 	  ; packet size 
	db 0x0  	  ; reserved
	dw sector_count; sector count
	dw 0x8000   ; offset to load to
	dw 0x0000   ; segment to load to 
	dq 1        ; starting LBA (this is 0, stage2 is 1)

; padding
times 510-($-$$) db 0
dw 0xaa55 ; bootable magic number




; END OF BOOT SECTOR
; ------------------------------------------------------------------------------
org 0x8000




init_stage2:
	; setup segments
	mov ax, 0
	mov ds, ax 
	mov es, ax
	jmp stage2

stage2:
	call clear
	mov bp, stage2msg
	mov cx, stage2msglen
	mov bl, 0x03 ; cyan
	mov dl, 31
	call showmessage

	call smap

	mov ax, 0
	mov ds, ax ; segment with DAP
	mov ah, 0x42 ; extended read
	mov dl, [driveindex]
	mov si, c_dap ; C_DAP structure
	int 0x13 ; do the read to load stage2 into RAM

	mov ax, 0
	mov ds, ax

	cli ; disable bios
	lgdt [gdtr]

	; turn on protected mode 
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp 0x08:init_protected

smap_in_hex = 0x534d4150
smap_amount = 0x2ff0

smap:
	mov [smap_amount], word 0
	mov di, 0x3000
	xor ebx, ebx
	mov edx, smap_in_hex
smap_next:
	clc
	mov eax, 0xe820
	mov ecx, 24
	int 0x15
	jc smap_done
	cmp eax, smap_in_hex
	jne smap_error
	add [smap_amount], word 1
	add di, 24
	test ebx, ebx 
	jnz smap_next
smap_done:
	ret
smap_error:
	call clear

	mov bp, smaperrmsg
	mov cx, smaperrmsglen
	mov bl, 0x84 ; red
	mov dl, 31
	call showmessage

	jmp $


gdt:
gdt_null:
	times 8 db 0
gdt_code:
	dw 0xffff ; first 16 bits of limit
	dw 0x0000 ; first 16 bits of base
	db 0x00   ; next 8 bits of base 
	db 10011010b ; access byte
	db 11001111b ; flags + last 4 bits of limit
	db 0x00   ; last 8 bits of base
gdt_data:
	dw 0xffff
	dw 0x0000
	db 0x00
	db 10010010b
	db 11001111b
	db 0x00

gdtr:
	dw 23
	dd gdt

align 4
c_dap:
	db 0x10 	  ; packet size 
	db 0x0  	  ; reserved
	dw sector_count_c; sector count
	dw 0x9000   ; offset to load to
	dw 0x0000   ; segment to load to 
	dq sector_count + 1 ; starting lba

stage2msg db "Stage 2 of Bootloader"
stage2msglen = $-stage2msg
smaperrmsg db "ERROR: SMAP error"
smaperrmsglen = $-smaperrmsg




; end of real mode
; ------------------------------------------------------------------------------




use32


init_protected:
	; set up segments, again
	mov ax, 0x10 ; data segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; set up stack, again
	mov esp, 0x90000
	mov ebp, esp

	call c_kernel
	jmp 0x9000

sector_count = (($-init_stage2)+511)/512
display "Reading ", sector_count + '0', " extra sectors from Disk", 10
times 512 * sector_count - ($-init_stage2) db 0

c_kernel:
	file "../build/kernel.bin"

sector_count_c = (($-c_kernel)+511)/512
display "Reading ", sector_count_c + '0', " extra C sectors from Disk", 10
times 512 * sector_count_c - ($-c_kernel) db 0

