#include "kmalloc.h"
#include "math.h"
#include "common.h"
#include "pfa.h"
#include "paging.h"

#define MINIMUM_UNITS_ALLOC 1024

static header_t base = { 0 }; 
static header_t * freeptr = NULL;

static void * sbrk(unsigned int bytes)
{
    // TODO: do not waste pages remainder
    unsigned int storage_vaddr = 0;
    bytes += bytes % PAGE_SIZE;
    storage_vaddr = paging_find_free_next_kernel_vaddr(PAGE_SIZE);
    for (unsigned int i = 0; i < bytes; i += PAGE_SIZE)
    {
        paging_commit_mem(storage_vaddr + i, pfa_alloc(1), PAGE_SIZE, PAGING_FLAGS_READ_WRITE | PAGING_FLAGS_PRIV_KERNEL);
    }
    return (void *)storage_vaddr;
}

static void * get_more_memory(unsigned int units)
{
    void * mem = NULL;
    header_t * hmem = NULL;
    if (MINIMUM_UNITS_ALLOC > units)
    {
        units = MINIMUM_UNITS_ALLOC;
    }
    mem = sbrk(units * sizeof(header_t));
    if ((void *)-1 == mem)
    {
        return NULL;
    }
    hmem = (header_t *)mem;
    hmem->size = units;
    kfree(hmem + 1);
    return freeptr;
}

void * kmalloc(unsigned int size)
{
    header_t * ptr = NULL;
    header_t * prevptr = NULL;
    unsigned int units_number = div_ceil(size, sizeof(header_t)) + 1;// number of units to be allocated

    if (freeptr == NULL)
    {
        // first use. initialize the list
        freeptr = &base;
        base.next = freeptr;
        base.size = 0;
    }

    prevptr = freeptr;
    for(ptr = freeptr->next; ; prevptr=ptr, ptr=ptr->next)
    {
        if (ptr->size >= units_number)
        {
            // founed pointer big enough
            if (ptr->size == units_number)
            {
                // exactly at the size
                prevptr->next = ptr->next;
            }
            else
            {
                // update the free pointer
                ptr->size -= units_number;
                ptr+=ptr->size;
                ptr->size = units_number;
            }
            freeptr = prevptr;
            return (void *)(ptr + 1);
        }
        if (ptr == freeptr)
        {
            // reached end of the list
            ptr = get_more_memory(units_number);
            if (ptr == NULL)
            {
                return NULL;
            }
            // more memory inserted into free list, iterate over it again
        }
    }
}

void * kcalloc(unsigned int size)
{
    void * ptr = kmalloc(size);
    if (ptr != NULL)
    {
        memset(ptr, 0, size);
    }
    return ptr;
}

void kfree(void * ptr)
{
    header_t * p = NULL; 
    // point to block header_t
    header_t * block_metadata = (header_t *)ptr - 1;

    if (NULL == ptr)
    {
        return;
    }

    for (p = freeptr; block_metadata > p && block_metadata < p->next  ; p = p->next)
    {
        if (p >= p->next &&
           (block_metadata > p || block_metadata < p->next))
        {
            /* freed block at start of end of arena */
            break;
        }
    }
     if (block_metadata + block_metadata->size == p->next) {
        /* join to upper nbr */
        block_metadata->size += p->next->size;
        block_metadata->next = p->next->next;
    } else {
        block_metadata->next = p->next;
    }

    if (p + p->size == block_metadata) {
        /* join to lower nbr */
        p->size += block_metadata->size;
        p->next = block_metadata->next;
    } else {
        p->next = block_metadata;
    }

    /* TODO: If the block is larger than a page frame, give back to pfa! */
    freeptr = p;
}