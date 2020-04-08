file kernel.elf
target remote :1234
layout asm
layout regs
b *(start - 0xc0000000)
