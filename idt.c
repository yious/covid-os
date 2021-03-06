#include "idt.h"
#include "io.h"
#include "serial.h"
#include "keyboard.h"
#include "framebuffer.h"

static struct idt idt = { 0 };
static struct idt_entry idt_entries[INTERRUPT_TABLE_LENGTH] = { 0 };
static interrupt_handler_callback interrupt_handlers[INTERRUPT_TABLE_LENGTH] = { 0 };

void idt_remap_pic()
{   
    /* remap the pic to another location*/
    /* ICW1 */
    outb(PIC1_PORT_A, 0x11); /* Master port A */
    outb(PIC2_PORT_A, 0x11); /* Slave port A */

    /* ICW2 */
    outb(PIC1_PORT_B, 0x20); /* Master offset of 0x20 in the IDT */
    outb(PIC2_PORT_B, 0x28); /* Master offset of 0x28 in the IDT */

    /* ICW3 */
    outb(PIC1_PORT_B, 0x04); /* Slaves attached to IR line 2 */
    outb(PIC2_PORT_B, 0x02); /* This slave in IR line 2 of master */

    /* ICW4 */
    outb(PIC1_PORT_B, 0x05 ); /* Set as master */
    outb(PIC2_PORT_B, 0x01); /* Set as slave */
}

void idt_init()
{
    unsigned int * interrupt_vector = (unsigned int *)interrupt_handlers_vector_start;
    unsigned int * interrupt_vector_end = (unsigned int *)interrupt_handlers_vector_end;
    struct idt_entry * current_entry = 0;

    idt_remap_pic();

    /* Configure the idt entries using interrupt_handlers */
    for (int i = 0; i < INTERRUPT_TABLE_LENGTH; i++)
    {
        current_entry = &idt_entries[i];
        if (interrupt_vector + i >= interrupt_vector_end){
            break;
        }
        /*
            flags:
            Bit:        | 15 | 14 13 | 12 | 11 | 10 9 8 | 7 6 5 | 4 3 2 1 0 |
            Content:    | P  | DPL   | 0  | D  | 1  1 0 | 0 0 0 | reserved  |
            In our case:| 1  |  0    | 0  | 1  | 1  1 0 | 0 0 0 | 0 0 0 0 0 | 
                        =0x2700
        */
        current_entry->flags = 0x8e00;
        current_entry->offset_high = ((unsigned int)interrupt_vector[i] >> 16) & 0xFFFF;
        current_entry->offset_low = ((unsigned int)interrupt_vector[i]) & 0xFFFF;
        current_entry->segment_selector = 0x08; // cs_kernel
    }
    

    idt.base = idt_entries;
    idt.limit = sizeof(idt_entries);
    /* load the idt */
    lidt(&idt);
}

int register_interrupt_handler(unsigned int code, interrupt_handler_callback cb)
{
    if (code >= INTERRUPT_TABLE_LENGTH)
    {
        serial_puts(SERIAL_COM1, "Tried to register invalid interrupt code\n");
        return 1;
    }
    if (interrupt_handlers[code] != 0)
    {
        serial_puts(SERIAL_COM1, "There is already interrupt code handler registered. code:");
        serial_put_hex(SERIAL_COM1, code);
        serial_puts(SERIAL_COM1, "\n");
        return 2;
    }

    interrupt_handlers  [code] = cb;
    return 0;
}

void interrupt_service_handler(struct cpu_state cpu, unsigned int interrupt, struct stack_state stack)
{
    int result = 0;
    if (interrupt_handlers[interrupt] != 0) 
    {
        result = interrupt_handlers[interrupt](cpu, interrupt, stack);
    }
    if (result != 0)
    {
        // Should do something
        serial_puts(SERIAL_COM1, "Interrupt failed"); 
    }
    pic_acknowledge(interrupt);
}

void pic_acknowledge(unsigned int interrupt)
{
    if (interrupt < PIC1_START_INTERRUPT || interrupt > PIC2_END_INTERRUPT) {
        return;
    }

    if (interrupt < PIC2_START_INTERRUPT)
    {
        outb(PIC1_PORT_A, PIC_ACK);
    }
    else
    {
        outb(PIC2_PORT_A, PIC_ACK);
    }
}