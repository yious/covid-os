#include "common.h"
#include "kmalloc.h"

unsigned long storage_pointer = (unsigned long)&_kernel_virtual_end;

static unsigned int kmalloc_internal(unsigned int size, unsigned int *paddr, BOOL is_aligned)
{
    unsigned int temp_addr = NULL;
    if (TRUE == is_aligned && !(storage_pointer & 0x00000FFF)) // check if needed to be aligned and not already aligned
    {   
        storage_pointer &= 0xFFFFF000;
        storage_pointer += 0x1000;
    }
    temp_addr = storage_pointer;
    if (NULL != paddr)
    {
        *paddr = temp_addr - 0xC0000000;
    }
    storage_pointer += size;
    return temp_addr;
}

unsigned int kmalloc_a(unsigned int size)
{
    return kmalloc_internal(size, NULL, TRUE);
}

unsigned int kmalloc_p(unsigned int size, unsigned int *paddr)
{
    return kmalloc_internal(size, paddr, FALSE);
}
unsigned int kmalloc_ap(unsigned int size, unsigned int *paddr) // page aligned and returns a physical address.
{
    return kmalloc_internal(size, paddr, TRUE);
}
unsigned int kmalloc(unsigned int size)
{
    return kmalloc_internal(size, NULL, FALSE);
}