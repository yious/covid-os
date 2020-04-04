global lgdt
global flush_selectors

lgdt:
    mov eax, [esp + 4] ; gdt struct address 
    lgdt [eax]
    ret

flush_selectors:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    jmp 0x08:flush_cs

flush_cs:
    ret