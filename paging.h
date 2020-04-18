#ifndef PAGING_H
#define PAGING_H

#include "common.h"
#include "idt.h"
#include <stdint.h>

#define PAGING_FLAGS_PRESENT        0x1
#define PAGING_FLAGS_NOT_PRESENT    0x0
#define PAGING_FLAGS_READ_WRITE     0x2
#define PAGING_FLAGS_READ_ONLY      0x0
#define PAGING_FLAGS_PRIV_USER      0x4
#define PAGING_FLAGS_PRIV_KERNEL    0x0

#define PAGE_SIZE                   0x1000
#define PAGE_TABLE_LEN              1024

#define PAGING_PAGE_SIZE_4KB 0
#define PAGING_PAGE_SIZE_4MB 1

struct pde 
{
	unsigned char present       : 1;
	unsigned char is_read_write	: 1;
	unsigned char is_user_only	: 1;
	unsigned char is_write_through : 1;
	unsigned char is_cache_disabled : 1;
	unsigned char accessed      : 1;
	unsigned char zero          : 1;
	unsigned char page_size     : 1;
	unsigned int rsvd           : 4;
	unsigned int page_address   : 20;

}__attribute__((packed));
typedef struct pde pde_t;

struct pte
{
	unsigned char present       : 1;
	unsigned char is_read_write	: 1;
	unsigned char is_user_only	: 1;
	unsigned char is_write_through : 1;
	unsigned char is_cache_disabled : 1;
	unsigned char accessed : 1;
	unsigned char dirty : 1;
	unsigned char zero : 1;
	unsigned char global : 1;
	unsigned int rsvd : 3;
	unsigned int page_address : 20;

}__attribute__((packed));
typedef struct pte pte_t;

extern int page_directory;

void paging_init(unsigned int pdt_vaddr, unsigned int pt_vaddr);
unsigned int paging_find_free_next_kernel_vaddr(unsigned int size);
unsigned int paging_commit_mem(unsigned int vaddr, unsigned int paddr, unsigned int size, unsigned int flags);
void paging_free_mem(unsigned int vaddr, unsigned int size);

extern unsigned int get_faulting_address();
extern void invalidate_vaddr(unsigned int page_vaddr);
extern void load_cr3(unsigned int pdt);
extern unsigned int read_cr3();

int pagefault_interrupt_handler(cpu_state_t cpu, unsigned int interrupt, stack_state_t stack);

#endif // PAGING_H