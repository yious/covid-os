SOURCES = $(wildcard *.c) $(wildcard *.s)
OBJECTS = $(patsubst %.s,%.o,$(SOURCES))
OBJECTS := $(patsubst %.c,%.o,$(OBJECTS))
HEADERS = $(wildcard *.h)
# OBJECTS = multiboot.o loader.o kmain.o framebuffer.o io.o serial.o lgdt.o gdt.o idt.o interrupt.o keyboard.o paging.o paging_asm.o timer.o common.o pfa.o pfa_asm.o
CC = gcc
CFLAGS = -m32 -nostdlib -fno-builtin -fno-stack-protector \
		 -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c -gdwarf -fno-pic
LD = ld
LFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf -g -F dwarf

all: os.iso

.PHONY: apps
apps:
	make -C apps

kernel.elf: $(OBJECTS) link.ld
	$(LD) $(LFLAGS) $(OBJECTS) -L "/usr/lib/gcc/x86_64-linux-gnu/7/32/" -lgcc -o kernel.elf 

os.iso: kernel.elf apps
	cp kernel.elf iso/boot/kernel.elf
	cp apps/program iso/modules/program
	genisoimage -R \
				-b boot/grub/stage2_eltorito \
				-no-emul-boot  \
				-boot-load-size 4 \
				-A os \
				-input-charset utf8 \
				-quiet \
				-boot-info-table \
				-o os.iso \
				iso

run: os.iso
	qemu-system-i386 -cdrom os.iso -serial stdio -d guest_errors

debug: os.iso
	qemu-system-i386 -cdrom os.iso -monitor stdio -s -S -d guest_errors
	

gdb: os.iso
	qemu-system-i386 -cdrom os.iso -serial stdio -s -S -d guest_errors &
	gdb -q -x gdbinit
	

%.o: %.c makefile $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s makefile
	$(AS) $(ASFLAGS) $< -o $@

clean:
	make -C apps clean
	rm -rf *.o kernel.elf os.iso
