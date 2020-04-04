FLAGS			equ	0
MAGIC			equ	0x1BADB002
CHECKSUM		equ	-(MAGIC + FLAGS)

section .multiboot
dd MAGIC
dd FLAGS
dd CHECKSUM