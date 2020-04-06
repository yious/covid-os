ALIGN_MODULES		equ 	0x00000001
FLAGS			equ	ALIGN_MODULES
MAGIC			equ	0x1BADB002
CHECKSUM		equ	-(MAGIC + FLAGS)

section .multiboot
dd MAGIC
dd FLAGS
dd CHECKSUM

section .text
global check_multiboot_flags
check_multiboot_flags:
    mov eax, [esp + 4] ; flags
    cmp eax, FLAGS
    jz flags_equal
    mov eax, 0; not equal
flags_equal:
    mov eax, 1
    ret