global get_faulting_address
get_faulting_address:
    mov eax, cr2
    ret

global invalidate_vaddr ; arg0 unsigned int page_vaddr
invalidate_vaddr:
    mov eax, [esp + 4]
    invlpg [eax]
    ret

global load_cr3
load_cr3:
    mov eax, [esp + 4]
    mov cr3, eax
    ret

global read_cr3
read_cr3:
    mov eax, cr3
    ret