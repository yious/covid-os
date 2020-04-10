#include "paging.h"
#include "serial.h"
#include "framebuffer.h"
#include "common.h"

void paging_init()
{

    register_interrupt_handler(PAGEFAULT_INT, pagefault_interrupt_handler);
}

int pagefault_interrupt_handler(cpu_state_t cpu, unsigned int interrupt, stack_state_t stack)
{
    UNUSED(cpu);
    UNUSED(interrupt);
    UNUSED(stack);

    serial_puts(SERIAL_COM1, "Pagefault. addr:");
    serial_put_hex(SERIAL_COM1, get_faulting_address());
    serial_puts(SERIAL_COM1, ". eip:");
    serial_put_hex(SERIAL_COM1, stack.eip);
    serial_puts(SERIAL_COM1, ". code:");
    serial_put_hex(SERIAL_COM1, stack.error_code);
    serial_puts(SERIAL_COM1, ". \n");
    fb_puts("Pagefault");

    PANIC("PAGEFAULT")

    return 0;
}
