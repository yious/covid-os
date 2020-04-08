global paging_enable
paging_enable:
;
; Software enables paging by using the MOV to CR0 instruction to set CR0.PG. Before doing so, software should
; ensure that control register CR3 contains the physical address of the first paging structure that the processor will
; use for linear-address translation (see Section 4.2) and that structure is initialized as desired
;
    ; Load pdt
    mov ebx, [esp + 4] ; pdt physical address
    mov cr3, ebx

    ; enable paging 
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    
    ret

global get_faulting_address
get_faulting_address:
    mov eax, cr2
    ret