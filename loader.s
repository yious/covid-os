STACK_SIZE      equ 16384 ; 16 kb
KERNEL_OFFSET   equ 0xC0000000
PAGE_SIZE       equ 4096

global start
extern kmain
extern _kernel_start
extern _kernel_end

section .text
bits 32
start:
    mov dword [0xb8000], 0x2f4b2f4f ; show that our code start running

    ; setup pte
    ; make both addresses 
    mov edi, boot_page_table1 - KERNEL_OFFSET ; boot_page_table1 physical address 
    mov esi, 0 ; points to the memory to be mapped 
    mov ecx, 1024
loop_start:
    ; map only the kernel
    cmp esi, _kernel_start - KERNEL_OFFSET ; were trying to map before the kernel. skip.
    jl loop_increment
    cmp esi, _kernel_end - KERNEL_OFFSET ; were trying to map after. finish the loop.
    jge after_loop

    ; this is valid kernel page which should be mapped
    mov edx, esi 
    or edx, 3 ; PAGING_RW | PAGING_PRESENT
    mov dword [edi], edx; write the entry back

loop_increment:
    ; increment edi (the pointer to the pte) and esi (the pointer to the memory)
    add edi, 4
    add esi, PAGE_SIZE ; page size

    loop loop_start
after_loop:
    ; init vga buffer
    mov esi, boot_page_table1 - KERNEL_OFFSET + 1023 * 4 ; point do the last entry
    mov dword [esi], 0x000B8000 | 0x3 ; 
    ; setup pde (initialize only the first one and the one in 0xc0000000)
    mov dword [boot_page_directory - KERNEL_OFFSET], boot_page_table1 - KERNEL_OFFSET + 0x3
    mov dword [boot_page_directory - KERNEL_OFFSET + 768 * 4], boot_page_table1 - KERNEL_OFFSET + 0x3

    ; enable paging
    ; load pde into cr3
    mov esi, dword boot_page_directory - KERNEL_OFFSET ; boot_page_directory physical address 
    mov cr3, esi
    ; set the 31bit in cr0 
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ; jump to the virtual address of the kernel 
    mov ecx, abs_jmp
    jmp ecx
abs_jmp:
    ; the identity paging is no longer necessary
    mov dword [boot_page_directory], 0

    invlpg [0] ; refresh the tlb

    mov esp, stack_start
    push ebx ; multiboot_info

    call kmain
loop:
    hlt
    jmp loop

section .bss
stack_end:
    resb STACK_SIZE
stack_start:

boot_page_directory:
    resb PAGE_SIZE
boot_page_table1:
    resb PAGE_SIZE