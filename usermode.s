USER_MODE_CODE_SEGMENT_SELECTOR equ 0x18
USER_MODE_DATA_SEGMENT_SELECTOR equ 0x20

global switch_to_um
switch_to_um: ; (unsigned int stack, unsigned int code)
	mov ebp, esp
	
	push USER_MODE_DATA_SEGMENT_SELECTOR | 0x3	; [esp + 16]  ss       the stack segment selector we want for user mode
	push dword [ebp + 4]						; [esp + 12]  esp      the user mode stack pointer
	push 0										; [esp +  8]  eflags   the control flags we want to use in user mode
	push USER_MODE_CODE_SEGMENT_SELECTOR | 0x3	; [esp +  4]  cs       the code segment selector
	push dword [ebp + 8]						; [esp +  0]  eip      the instruction pointer of user mode code to execute

	mov ax, USER_MODE_DATA_SEGMENT_SELECTOR | 0x3
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
	
	iret