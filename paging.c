#include "paging.h"


void setup_pde(unsigned int virt, unsigned int phys, unsigned short flags)
{
    unsigned int * pde = (unsigned int *)pde_start;
    unsigned short id = virt >> 22;
    pde[id] = 0 | flags;
    if (flags & PAGING_PAGE_SIZE_4MB)
    {
        pde[id] |= phys & 0b11111111110000000000000000000000;
    }
    else
    {
        //
        // pde[id] = 
    }
}

// static pde_t pde[IDENTITY_PAGING_PDE_SIZE] = {0}; 
void setup_pde_old(
    pde_t * out, // should be zeroed out
    unsigned char present, // present
    unsigned char is_rw, // read/write
    unsigned char priv, // user/supervisor
    unsigned char pwt, // page-level write-through
    unsigned char pcd, // page level cache disable
    unsigned char page_size, // page size 
    unsigned char global,
    unsigned int address // page-table base address/page base address
    )
{
    out->flags |= present & PAGING_PRESENT;
    out->flags |= is_rw & PAGING_RW;
    out->flags |= priv & PAGING_PRIV;
    out->flags |= pwt & PAGING_PWT;
    out->flags |= pcd & PAGING_PCD;
    out->flags |= page_size & PAGING_PAGE_SIZE;

    (void)global;

    if (page_size == PAGING_PAGE_SIZE_4KB)
    {
        // bits 12-31: page-table base address
        out->middle |= (address & 0xF ) << 12;
        out->high_address = address >> 4;
    } 
    else 
    {
        // PAGING_4MB
        // bit 12: pat (0)
        // bits 13-21: reserved (0)
        // bits 22-31: page base address
        out->high_address |= address << 6;
    }
}

void paging_init(void)
{
    /* Configure 1000 PDE's */ 
    pde_t * pde = (pde_t *)pde_start;
    for (unsigned int i = 0; i < IDENTITY_PAGING_PDE_SIZE; i++)
    {
        setup_pde(i << 22, i << 22 , PAGING_PRESENT | PAGING_RW | PAGING_PRIVLEGE_SUPERVISOR | PAGING_PAGE_SIZE_4MB);

    }
    
    /* */
    paging_enable(pde);
}