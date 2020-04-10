#include "pfa.h"
#include "common.h"
#include "serial.h"

void pfa_init(multiboot_info_t * mbinfo)
{
    mmap_entry_t * entry = 0;
    unsigned int size = 0; 
    unsigned long ram_total = 0;
    unsigned int array_len = 0;
    // figure how much memory is available
    
    serial_puts(SERIAL_COM1, "Kernel ending at physical address: ");
    serial_put_hex(SERIAL_COM1, (unsigned int)&_kernel_physical_end);
    serial_puts(SERIAL_COM1, "\n");


    if (IS_BIT_SET(mbinfo->flags, 6) == 0)
    {
        PANIC("Could not figure out how much RAM available.")
    }

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

        size += entry->size + sizeof(entry->size);
        ram_total += entry->length;
    }

    serial_puts(SERIAL_COM1, "Ram total size: ");
    serial_put_dec(SERIAL_COM1, ram_total / (1024 * 1024));
    serial_puts(SERIAL_COM1, " MB\n");
    // create bitmap after the kernel to store information about it
    /* each page is 1 bit thus we can fit 32 pages per int  */
    array_len = ram_total / 32;
    pfa_setup(&_kernel_physical_end, array_len);
    return;
}

// len is is length of the array (NOT SIZE IN BYTES)
void pfa_setup(unsigned int * storage, unsigned int len)
{
    serial_puts(SERIAL_COM1, "pfa storage location: ");
    serial_put_hex(SERIAL_COM1, (unsigned int)storage);
    serial_puts(SERIAL_COM1, " Size: ");
    serial_put_dec(SERIAL_COM1, (unsigned int)len);
    serial_puts(SERIAL_COM1, "\n");
    // allocate enough memory in order to contain it (which is (len*sizeof(storage[0]))/PAGE_SIZE)
}