OBJECTS = multiboot.o loader.o kmain.o framebuffer.o io.o serial.o lgdt.o gdt.o idt.o interrupt.o keyboard.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
		 -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c -g
LD = ld
LFLAGS = -T link.ld -melf_i386
AS = nasm -g
ASFLAGS = -f elf

all: os.iso

kernel.elf: $(OBJECTS)
		$(LD) $(LFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
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
	qemu-system-i386 -cdrom os.iso -serial stdio -s -S -d guest_errors &
	gdb -q -x gdbinit
	

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf os.iso
