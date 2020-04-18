#include "paging.h"
#include "serial.h"
#include "framebuffer.h"
#include "common.h"
#include "pfa.h"
#include "math.h"

#define PDE_MAGIC_ENTRY 1023
#define PDE_MAGIC_OFFSET PDE_MAGIC_ENTRY << 22 // PDE_MAGIC_ENTRY * sizeof(pde_t) 

static unsigned int * pdt;

void paging_init(unsigned int pdt_vaddr, unsigned int pt_vaddr)
{
    UNUSED(pt_vaddr);
    pdt = (unsigned int *)pdt_vaddr;
    pdt[1023] = read_cr3() | PAGING_FLAGS_PRESENT | PAGING_FLAGS_READ_WRITE | PAGING_FLAGS_PRIV_KERNEL;
    register_interrupt_handler(PAGEFAULT_INT, pagefault_interrupt_handler);
}

unsigned int paging_find_free_next_kernel_vaddr(unsigned int size)
{
    pde_t * current_pde = NULL;
    pte_t * current_pte = NULL;
    unsigned int kernel_start_idx = 0xC0000000 >> 22;
    unsigned int free_vaddr = 0;
    unsigned int pt_base = 0;
    unsigned int empty_pages = 0;
    UNUSED(size);
    UNUSED(current_pde);
    // loop all the page directory entries and look for available one
    for (unsigned int i = kernel_start_idx; i < PAGE_TABLE_LEN; i++)
    {
        current_pde = (pde_t *)pdt + i;
        if (current_pde->present == FALSE) // found empty pde (4mb available)
        {
            free_vaddr = i << 22;
            return free_vaddr;
        }
        // this pde is present, search inside it
        pt_base = PDE_MAGIC_OFFSET;
        pt_base += i << 12; // the offset of the pde we want to search
        for (unsigned int j = 0; j < PAGE_TABLE_LEN; j++)
        {
            current_pte = (pte_t *)(pt_base + j * sizeof(pde_t));
            if (current_pte->present == FALSE)
            {
                // found empty pte
                empty_pages += 1;
                if (empty_pages * PAGE_SIZE >= size)
                {
                    free_vaddr = i << 22;
                    free_vaddr += (j - (empty_pages - 1)) << 12;
                    return free_vaddr;
                }
            }
            else
            {
                empty_pages = 0;
            }
        }  
    }
    PANIC("paging_find_free_next_kernel_vaddr: no virtual address available.")
    return 0x0BADC0DE;
}

static void set_pde(pde_t * out, unsigned int page_paddr, unsigned int flags)
{
    memset(out, 0, sizeof(pde_t));
    if (flags & PAGING_FLAGS_PRESENT)
    {
        out->present = TRUE;
    }
    if (flags & PAGING_FLAGS_READ_WRITE)
    {
        out->is_read_write = TRUE;
    }
    if (flags & PAGING_FLAGS_PRIV_USER)
    {
        out->is_user_only = TRUE;
    }
    out->page_address = page_paddr >> 12;
}

static void set_pte(pte_t * out, unsigned int page_paddr, unsigned int flags)
{
    memset(out, 0, sizeof(pte_t));
    if (flags & PAGING_FLAGS_PRESENT)
    {
        out->present = TRUE;
    }
    if (flags & PAGING_FLAGS_READ_WRITE)
    {
        out->is_read_write = TRUE;
    }
    if (flags & PAGING_FLAGS_PRIV_USER)
    {
        out->is_user_only = TRUE;
    }
    out->page_address = page_paddr >> 12;
}

static pte_t * get_pt(unsigned int pdt_idx)
{
    unsigned addr = PDE_MAGIC_OFFSET;
    addr += pdt_idx << 12;
    return (pte_t *)addr;
}

/*
    map vaddr to point at paddr

    Algorithm as follows:
    1. Split vaddr into pdt index and pt index
    2. if pdt doesn't exist,
        a. allocate one physical page
        b. assign the physical address to the pde and mark it as present
    3. using the magic pde, access the page table
    4. starting at 'pt_index' loop 'size/PAGE_SIZE' times and mark the following pages as present
 */
unsigned int paging_commit_mem(unsigned int vaddr, unsigned int paddr, unsigned int size, unsigned int flags)
{
    unsigned int pdt_idx = vaddr >> 22;
    unsigned int pt_idx = (0x003FF000 & vaddr) >> 12;
    unsigned int page_paddr = 0;
    pde_t * target_pde = (pde_t *)pdt + pdt_idx;
    pte_t * target_pte = NULL;
    unsigned int pages_needed = div_ceil(size, PAGE_SIZE);
    if (target_pde->present == FALSE)
    {
        // no page table alloacted
        page_paddr = pfa_alloc(1);
        set_pde(target_pde, page_paddr, PAGING_FLAGS_PRESENT | PAGING_FLAGS_READ_WRITE | (flags & PAGING_FLAGS_PRIV_USER));
    }
    
    target_pte = get_pt(pdt_idx) + pt_idx;
    for (unsigned int i = 0; i < pages_needed; i++)
    {
        set_pte(target_pte + i, paddr + i * PAGE_SIZE, PAGING_FLAGS_PRESENT | flags);
    }
    return vaddr;
}
void paging_free_mem(unsigned int vaddr, unsigned int size)
{
    unsigned int pdt_idx = vaddr >> 22;
    unsigned int pt_idx = (0x003FF000 & vaddr) >> 12;
    pde_t * target_pde = (pde_t *)pdt + pdt_idx;
    pte_t * page_table = get_pt(pdt_idx);
    pte_t * target_pte = page_table + pt_idx;
    unsigned int  pages_needed = div_ceil(size, PAGE_SIZE);

    for (unsigned int i = 0; i < pages_needed; i++)
    {
        target_pte->present = FALSE;
    }

    /* check if pt empty */
    for (unsigned int i = 0; i < PAGE_TABLE_LEN; i++)
    {
        if (page_table[i].present == TRUE)
        {
            return; 
        }
    }
    pfa_free(target_pde->page_address << 12);    
    target_pte->present = FALSE;
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
