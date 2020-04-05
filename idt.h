#ifndef IDT_H
#define IDT_H

#define PIC1_PORT_A 0x20
#define PIC1_PORT_B 0x21
#define PIC2_PORT_A 0xA0
#define PIC2_PORT_B 0xA1

#define UNUSED_PARAM(param) (void)(param)

// as declared in interrupt.s
#define INTERRUPT_TABLE_LENGTH 256

#define PIC1_PORT_A 0x20
#define PIC2_PORT_A 0xA0

    /* The PIC interrupts have been remapped */
#define PIC1_START_INTERRUPT 0x20
#define PIC2_START_INTERRUPT 0x28
#define PIC2_END_INTERRUPT   PIC2_START_INTERRUPT + 7

#define PIC_ACK     0x20

/* INTERRUPTS */
#define KEYBOARD_INT 0x21

struct idt_entry {
    unsigned short offset_low;
    unsigned short segment_selector;
    unsigned short flags;
    unsigned short offset_high;
} __attribute__((packed));

struct cpu_state {
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    unsigned int esp;
    unsigned int ebp;
    unsigned int esi;
    unsigned int edi;
} __attribute__((packed));

struct stack_state {
    unsigned int error_code;
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;
} __attribute__((packed));

struct idt {
    unsigned short limit;
    struct idt_entry * base;
}__attribute__((packed));

extern void lidt(struct idt * idt); 

/* from interrupt.s */ 
void interrupt_handlers_vector_start();
void interrupt_handlers_vector_end();

void interrupt_handler(struct cpu_state cpu, unsigned int interrupt, struct stack_state stack);

void idt_init();

/** pic_acknowledge:
*  Acknowledges an interrupt from either PIC 1 or PIC 2.
*
*  @param num The number of the interrupt
*/
void pic_acknowledge(unsigned int interrupt);

#endif // IDT_H