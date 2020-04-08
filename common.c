#include "common.h"
#include "framebuffer.h"

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