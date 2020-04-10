glboal get_memory_map
get_memory_map:
    mov eax, 0xE820
    int 15h
    ret
