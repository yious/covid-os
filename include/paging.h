#ifndef PAGING_H
#define PAGING_H

#include "common.h"
#include "idt.h"

#define PAGING_FLAGS_PRESENT        0x1
#define PAGING_FLAGS_NOT_PRESENT    0x0
#define PAGING_FLAGS_READ_WRITE     0x2
#define PAGING_FLAGS_READ_ONLY      0x0
#define PAGING_FLAGS_PRIV_USER      0x4
#define PAGING_FLAGS_PRIV_KERNEL    0x0

void paging_init();

extern void paging_enable(unsigned int * base);

extern unsigned int get_faulting_address();

int pagefault_interrupt_handler(cpu_state_t cpu, unsigned int interrupt, stack_state_t stack);

#endif // PAGING_H