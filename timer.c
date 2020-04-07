#include "common.h"
#include "timer.h"
#include "io.h"
#include "framebuffer.h"

static unsigned int timer_tick_count = 0;

void timer_init(unsigned int freq)
{
    unsigned int div = FREQUENCY_MAX / freq;
    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    unsigned char low = (unsigned char)(div & 0xFF);
    unsigned char high = (unsigned char)( (div>>8) & 0xFF );
    
    register_interrupt_handler(TIMER_INT, timer_interrupt_handler); 

    // configure timer 0
    outb(0x43, 0x36);
    // Send the frequency divisor.
    outb(0x40, low);
    outb(0x40, high);
}

int timer_interrupt_handler(cpu_state_t cpu, unsigned int code, stack_state_t stack)
{
    UNUSED(cpu);
    UNUSED(code);
    UNUSED(stack);

    timer_tick_count += 1;

    return 0;
}