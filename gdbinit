target remote :1234
file kernel.elf
# add-symbol-file kernel.elf 0x00101000 
layout asm
layout regs

b start
