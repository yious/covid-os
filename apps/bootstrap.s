extern main

section .text
global start
start:
    ; TODO: implement argc, argv
    push 0 ; argv
    push 0 ; argc
    call main
loop:
    jmp loop