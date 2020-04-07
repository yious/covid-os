#include "idt.h"
#include "keyboard.h"
#include "framebuffer.h"
#include "common.h"

void keyboard_init()
{
    register_interrupt_handler(KEYBOARD_INT, keyboard_interrupt_handler);
}

int keyboard_interrupt_handler(cpu_state_t cpu, unsigned int code, stack_state_t stack)
{
    UNUSED(cpu);
    UNUSED(code);
    UNUSED(stack);

    char ch = scan_code_to_ascii(read_scan_code());
    fb_putch(ch);
    return 0;
}

unsigned char read_scan_code(void)
{
    return inb(KBD_DATA_PORT);
}

char scan_code_to_ascii(unsigned char scan_code)
{
    char ch = -1;
    char is_break = 0x80 & scan_code; /* if msb is set it's break, otherwise make. */
    if (is_break != 0)
    {
        return -1;
    }
     switch (scan_code) {
        case KBD_SC_A:
            ch = 'a';
            break;
        case KBD_SC_B:
            ch = 'b';
            break;
        case KBD_SC_C:
            ch = 'c';
            break;
        case KBD_SC_D:
            ch = 'd';
            break;
        case KBD_SC_E:
            ch = 'e';
            break;
        case KBD_SC_F:
            ch = 'f';
            break;
        case KBD_SC_G:
            ch = 'g';
            break;
        case KBD_SC_H:
            ch = 'h';
            break;
        case KBD_SC_I:
            ch = 'i';
            break;
        case KBD_SC_J:
            ch = 'j';
            break;
        case KBD_SC_K:
            ch = 'k';
            break;
        case KBD_SC_L:
            ch = 'l';
            break;
        case KBD_SC_M:
            ch = 'm';
            break;
        case KBD_SC_N:
            ch = 'n';
            break;
        case KBD_SC_O:
            ch = 'o';
            break;
        case KBD_SC_P:
            ch = 'p';
            break;
        case KBD_SC_Q:
            ch = 'q';
            break;
        case KBD_SC_R:
            ch = 'r';
            break;
        case KBD_SC_S:
            ch = 's';
            break;
        case KBD_SC_T:
            ch = 't';
            break;
        case KBD_SC_U:
            ch = 'u';
            break;
        case KBD_SC_V:
            ch = 'v';
            break;
        case KBD_SC_W:
            ch = 'w';
            break;
        case KBD_SC_X:
            ch = 'x';
            break;
        case KBD_SC_Y:
            ch = 'y';
            break;
        case KBD_SC_Z:
            ch = 'z';
            break;
        case KBD_SC_0:
            ch = '0';
            break;
        case KBD_SC_1:
            ch = '1';
            break;
        case KBD_SC_2:
            ch = '2';
            break;
        case KBD_SC_3:
            ch = '3';
            break;
        case KBD_SC_4:
            ch = '4';
            break;
        case KBD_SC_5:
            ch = '5';
            break;
        case KBD_SC_6:
            ch = '6';
            break;
        case KBD_SC_7:
            ch = '7';
            break;
        case KBD_SC_8:
            ch = '8';
            break;
        case KBD_SC_9:
            ch = '9';
            break;
        case KBD_SC_ENTER:
            ch = '\n';
            break;
        case KBD_SC_SPACE:
            ch = ' ';
            break;
        case KBD_SC_BS:
            ch = 8;
            break;
        case KBD_SC_DASH:
            ch = '-';
            break;
        case KBD_SC_EQUALS:
            ch = '=';
            break;
        case KBD_SC_LBRACKET:
            ch = '[';
            break;
        case KBD_SC_RBRACKET:
            ch = ']';
            break;
        case KBD_SC_BSLASH:
            ch = '\\';
            break;
        case KBD_SC_SCOLON:
            ch = ';';
            break;
        case KBD_SC_QUOTE:
            ch = '\'';
            break;
        case KBD_SC_COMMA:
            ch = ',';
            break;
        case KBD_SC_DOT:
            ch = '.';
            break;
        case KBD_SC_FSLASH:
            ch = '/';
            break;
        case KBD_SC_TILDE:
            ch = '`';
            break;
        case KBD_SC_TAB:
            ch = '\t';
            break;
     }
    return ch;
}