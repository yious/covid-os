#include "paging.h"
#include "serial.h"
#include "framebuffer.h"
#include "common.h"

static unsigned int * page_directory = (unsigned int *)0x9C000;
static unsigned int * page_table = (unsigned int *)0x9D000;

void paging_init()
{
    /* Fill out the first 4MB */
    // Fill the first pde
    page_directory[0] = (unsigned int)page_table | PAGING_FLAGS_PRESENT | PAGING_FLAGS_READ_WRITE | PAGING_FLAGS_PRIV_KERNEL;

    // fill the appropriate pte
    for(int i = 0; i < 1024; i++)
    {
        page_table[i] = i * 4096 | PAGING_FLAGS_PRESENT | PAGING_FLAGS_READ_WRITE;
    }

    // fill the rest of the pde with non allocated pages
    for (unsigned int i = 1; i < 1024; i++)
    {
        page_directory[i] = 0 | PAGING_FLAGS_NOT_PRESENT | PAGING_FLAGS_READ_WRITE;
    }
    register_interrupt_handler(PAGEFAULT_INT, pagefault_interrupt_handler);
    paging_enable(page_directory);
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
