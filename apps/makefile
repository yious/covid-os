CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Werror -c -fno-pic
LD = ld
LFLAGS = -T user.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: program

program: program.o bootstrap.o
	$(LD) $(LFLAGS) program.o bootstrap.o -o program

%.o: %.c makefile
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s makefile
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm *.o program
