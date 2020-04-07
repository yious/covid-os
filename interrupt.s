extern interrupt_service_handler

%macro no_error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    push    dword 0                     ; push 0 as error code
    push    dword %1                    ; push the interrupt number
    jmp     common_interrupt_handler    ; jump to the common handler
%endmacro

%macro error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    push    dword %1                    ; push the interrupt number
    jmp     common_interrupt_handler    ; jump to the common handler
%endmacro

common_interrupt_handler:               ; the common parts of the generic interrupt handler
    cli
    ; save the registers
    push edi
    push esi
    push ebp
    push esp
    push edx
    push ecx
    push ebx
    push eax

    ; call the C function
    call    interrupt_service_handler
    ; restore the registers
    pop eax
    pop ebx
    pop ecx
    pop edx
    pop esp
    pop ebp
    pop esi
    pop edi

    ; restore the esp
    add     esp, 8
    ; return to the code that got interrupted
    sti
    iret

; protected mode exceptions
no_error_code_interrupt_handler 0       ; create handler for interrupt 0 
no_error_code_interrupt_handler 1       ; create handler for interrupt 1 
no_error_code_interrupt_handler 2       ; create handler for interrupt 2 
no_error_code_interrupt_handler 3       ; create handler for interrupt 3 
no_error_code_interrupt_handler 4       ; create handler for interrupt 4 
no_error_code_interrupt_handler 5       ; create handler for interrupt 5 
no_error_code_interrupt_handler 6       ; create handler for interrupt 6 
no_error_code_interrupt_handler 7       ; create handler for interrupt 7 
error_code_interrupt_handler    8       ; create handler for interrupt 8 
no_error_code_interrupt_handler 9       ; create handler for interrupt 9 
error_code_interrupt_handler    10      ; create handler for interrupt 10
error_code_interrupt_handler    11      ; create handler for interrupt 11
error_code_interrupt_handler    12      ; create handler for interrupt 12
error_code_interrupt_handler    13      ; create handler for interrupt 13
error_code_interrupt_handler    14      ; create handler for interrupt 14
error_code_interrupt_handler    15      ; create handler for interrupt 15
no_error_code_interrupt_handler 16      ; create handler for interrupt 16
error_code_interrupt_handler    17      ; create handler for interrupt 17
no_error_code_interrupt_handler 18      ; create handler for interrupt 18
no_error_code_interrupt_handler 19      ; create handler for interrupt 19
no_error_code_interrupt_handler 20      ; create handler for interrupt 20

; irqs 
no_error_code_interrupt_handler 32 
no_error_code_interrupt_handler 33
no_error_code_interrupt_handler 34
no_error_code_interrupt_handler 35
no_error_code_interrupt_handler 36
no_error_code_interrupt_handler 37
no_error_code_interrupt_handler 38
no_error_code_interrupt_handler 39
no_error_code_interrupt_handler 40
no_error_code_interrupt_handler 41
no_error_code_interrupt_handler 42
no_error_code_interrupt_handler 43
no_error_code_interrupt_handler 44
no_error_code_interrupt_handler 45
no_error_code_interrupt_handler 46
no_error_code_interrupt_handler 47


global interrupt_handlers_vector_start
global interrupt_handlers_vector_end
interrupt_handlers_vector_start:
dd interrupt_handler_0
dd interrupt_handler_1
dd interrupt_handler_2
dd interrupt_handler_3
dd interrupt_handler_4
dd interrupt_handler_5
dd interrupt_handler_6
dd interrupt_handler_7
dd interrupt_handler_8
dd interrupt_handler_9
dd interrupt_handler_10
dd interrupt_handler_11
dd interrupt_handler_12
dd interrupt_handler_13
dd interrupt_handler_14
dd interrupt_handler_15
dd interrupt_handler_16
dd interrupt_handler_17
dd interrupt_handler_18
dd interrupt_handler_19
dd interrupt_handler_20
dd 0 ; 21
dd 0 ; 22
dd 0 ; 23
dd 0 ; 24
dd 0 ; 25
dd 0 ; 26
dd 0 ; 27
dd 0 ; 28
dd 0 ; 29
dd 0 ; 30
dd 0 ; 31
dd interrupt_handler_32
dd interrupt_handler_33
dd interrupt_handler_34
dd interrupt_handler_35
dd interrupt_handler_36
dd interrupt_handler_37
dd interrupt_handler_38
dd interrupt_handler_39
dd interrupt_handler_40
dd interrupt_handler_40
dd interrupt_handler_41
dd interrupt_handler_42
dd interrupt_handler_43
dd interrupt_handler_44
dd interrupt_handler_45
dd interrupt_handler_46
dd interrupt_handler_47
interrupt_handlers_vector_end:

global lidt

lidt:
    mov eax, [esp + 4]
    lidt [eax]
    sti
    ret