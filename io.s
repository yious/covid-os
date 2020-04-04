global outb
global inb

outb:
    mov dx, [esp + 4] ; port
    mov al, [esp + 8] ; data  
    out dx, al
    ret

inb:
    mov dx, [esp + 4] ; port
    in al, dx
    ret