#include "common.h"
#include "framebuffer.h"

unsigned int physical_to_virtual(unsigned int paddr)
{
    // TODO: make magic number a constant
    return 0xc0000000 + paddr;
}


void panic(char *message, char *file, unsigned int line)
{
    asm volatile("cli"); // Disable interrupts.

    fb_clear();

    fb_puts("PANIC(");
    fb_puts(message);
    fb_puts(") at ");
    fb_puts(file);
    fb_puts(":");
    fb_put_dec(line);
    fb_puts("\n");
    // Halt by going into an infinite loop.
    for(;;);
}