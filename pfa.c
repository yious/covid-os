#include "pfa.h"
#include "kmalloc.h"
#include "common.h"
#include "paging.h"
#include "serial.h"
#include "math.h"

struct pfa_storage 
{
    unsigned int * bitmap;
    unsigned int paddr;
    unsigned int size; // size of the array in bits
};
struct pfa_storage pfa_storage;

static void set_frame(unsigned int bit_index)
{
    unsigned int arr_index = bit_index / 32;
    unsigned char arr_offset = bit_index % 32;
    pfa_storage.bitmap[arr_index] |= 1 << arr_offset;
}

unsigned int get_ram_size(multiboot_info_t * mbinfo)
{
    mmap_entry_t * entry = 0;
    unsigned int size = 0;
    unsigned int ram_total = 0;

    if (IS_BIT_SET(mbinfo->flags, 6) == 0)
    {
        PANIC("PFA: Could not figure out how much RAM available.")
    }

    // figure how much memory is available so we would know how much to allocate
    while(size < mbinfo->mmap_length)
    {
        entry = (mmap_entry_t *)((unsigned int)mbinfo->mmap_addr + size);
        ram_total += entry->length;
        
        // next entry
        size += entry->size + sizeof(entry->size);
    }
    return ram_total;
}

void pfa_init(multiboot_info_t * mbinfo, kernel_memory_info_t * kmem)
{   
    unsigned int ram_size = 0; 
    mmap_entry_t * entry = 0;
    unsigned int size = 0;
    unsigned int rounded_base = 0;
    unsigned int rounded_length = 0;
    unsigned int bitmap_length = 0;
    unsigned int bitmap_size = 0;
    unsigned int bitmap_vaddr = 0;
    unsigned int ram_index = 0;
    unsigned int bitmap_bits_length = 0;

    ram_size = get_ram_size(mbinfo);
    serial_puts(SERIAL_COM1, "Ram total size: ");
    serial_put_dec(SERIAL_COM1, ram_size / (1024 * 1024));
    serial_puts(SERIAL_COM1, " MB\n");
    
    // allocate enough space
    bitmap_bits_length = ram_size / PAGE_SIZE;
    bitmap_length = div_ceil(bitmap_bits_length, 32); // if number of bits doens't divded by 32 add an extra element
    bitmap_size = bitmap_length * sizeof(*pfa_storage.bitmap); // in bytes 
    bitmap_vaddr = paging_find_free_next_kernel_vaddr(bitmap_size);
    paging_commit_mem(bitmap_vaddr, kmem->_kernel_physical_end, bitmap_size, PAGING_FLAGS_READ_WRITE | PAGING_FLAGS_PRIV_KERNEL); 
    memset((void *)bitmap_vaddr, 0, bitmap_size);
    pfa_storage.bitmap =  (unsigned int *)bitmap_vaddr;
    pfa_storage.size = bitmap_bits_length; 

    // populate the bitmap according to the information
    while(size < mbinfo->mmap_length)
    {
        entry = (mmap_entry_t *)((unsigned int)mbinfo->mmap_addr + size);
        serial_puts(SERIAL_COM1, "RAM: addr: ");
        serial_put_hex64(SERIAL_COM1, entry->base);
        serial_puts(SERIAL_COM1, " size: ");
        serial_put_hex64(SERIAL_COM1, entry->length);
        serial_puts(SERIAL_COM1, " type: ");
        serial_put_hex(SERIAL_COM1, entry->type);
        serial_puts(SERIAL_COM1, "\n");
        if (entry->type != 0x1 && entry->base >= kmem->_kernel_physical_end)
        {
            // invalid memory, mark as occupied
            // we need to round it down to the closest page (0xFFFFF000 + 0x1000)
            rounded_base = entry->base & 0xFFFFF000;
            rounded_length = div_ceil_long_long(entry->length, PAGE_SIZE); // we need to round it up to the closest page size (0x1000 : PAGE_SIZE)
            for(unsigned int i = 0; i < rounded_length; i ++)
            {
                ram_index = rounded_base / PAGE_SIZE + i;
                if (ram_index >= pfa_storage.size)
                {
                    break;
                }
                set_frame(rounded_base / PAGE_SIZE); 
            }
        }
        // next entry
        size += entry->size + sizeof(entry->size);
    }

    // set all the page untill the end of kmalloc as occupied
    for (unsigned int i = 0; i < kmem->_kernel_physical_end; i+=PAGE_SIZE)
    {
        set_frame(i / PAGE_SIZE);
    }
    return;
}


unsigned int pfa_alloc(unsigned int num)
{
    unsigned int array_len = div_ceil(pfa_storage.size, 8 * sizeof(*pfa_storage.bitmap));
    unsigned int canidate = 0xFFFFFFFF;
    unsigned int pages_found = 0; 
    unsigned int start_paddr = 0;

    for (unsigned int i = 0; i < array_len; i++)
    {
        canidate = pfa_storage.bitmap[i];
        if(canidate != 0xFFFFFFFF)
        {
            // there is turned of bit in this element
            for (unsigned char j = 0; j < 8 * sizeof(*pfa_storage.bitmap); j++)
            {
                if ((canidate & 1 << j) == 0)
                {
                    start_paddr = ((i * 32) + j) * PAGE_SIZE;
                    pages_found += 1;
                    if (pages_found == num)
                    {
                        for(unsigned int k=start_paddr/PAGE_SIZE; k < start_paddr/PAGE_SIZE + pages_found; k++)
                        {
                            set_frame(k);
                        }
                        return start_paddr;
                    }
                }
                else
                {
                    start_paddr = 0; 
                    pages_found = 0;
                }
            }
        }
            
    }

    PANIC("PFA: No more frames left!");
    return 0x0BADC0DE;
}

void pfa_free(unsigned int paddr)
{
    unsigned int index = paddr / 0x1000; // PAGE_SIZE
    unsigned int arr_index = index / 32;
    unsigned char arr_offset = index % 32;
    pfa_storage.bitmap[arr_index] = pfa_storage.bitmap[arr_index] & ~(1 << arr_offset);
}