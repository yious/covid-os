STACK_SIZE equ 4096

global start
extern kmain

section .text
bits 32
start:
mov eax, 0xCAFEBABE
mov dword [0xb8000], 0x2f4b2f4f
mov esp, stack_start
call kmain
hlt

section .bss
stack_end:
resb STACK_SIZE
stack_start: