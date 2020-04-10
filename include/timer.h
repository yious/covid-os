#ifndef TIMER_H
#define TIMER_H

#include "idt.h"

#define FREQUENCY_MAX 1193180
#define TIMER_PORT1 0x40
#define TIMER_PORT2 0x41
#define TIMER_PORT3 0x42
#define TIMER_CONTROL 0x43


void timer_init(unsigned int freq);

int timer_interrupt_handler(cpu_state_t cpu, unsigned int code, stack_state_t stack);

#endif // TIMER_H