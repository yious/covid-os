#include "gdt.h"

#define GDT_ENTRIES_LENGTH 3

static struct gdt gdt = { 0 };
static struct gdt_entry gdt_entries[GDT_ENTRIES_LENGTH] = { 0 };

void gdt_init()
{
    /*
        configure bit fields
        32bit protected mode with Granularity of 4kiB
        Flags   |gr|sz|0|0|
                |1 |1 | | | =0xC
        
                | 7  | 6 | 5 | 4 | 3  | 2  | 1  | 0 | 
        Access  | Pr | Privl | s | Ex | DC | RW | Ac|
        Pr - Present bit
        Privl - Ring level
        S - type 1 for code or data
        Ex - executable bit
        DC - Direction bit (data) / Coforming bit (code)
        RW - Read\Write
        Ac - access bit
    */

    const unsigned char cs_access = 0b10011010; // 0x9A
    const unsigned char ds_access = 0b10010010; // 0x92
    const unsigned char flags = 0b1100;
    gdt_load_entry(0, 0, 0, 0, 0); // Null segment
    gdt_load_entry(1, 0x000FFFFF, 0, cs_access, flags); // kernel cs segment
    gdt_load_entry(2, 0x000FFFFF, 0, ds_access, flags); // kernel ds segment

    gdt.address = gdt_entries;
    gdt.size = sizeof(gdt_entries);

    lgdt(&gdt);    

    flush_selectors(); 
}

int gdt_load_entry(
    unsigned short index,
    unsigned int limit,
    unsigned int base,
    unsigned char access,
    unsigned char flags
)
{
    struct gdt_entry * entry = 0;
    if (index >= GDT_ENTRIES_LENGTH)
    {
        return -1;
    }
    entry = & gdt_entries[index];
    
    entry->limit = limit & 0x0000FFFF;
    entry->base = base & 0x0000FFFF;
    entry->base2 = base & 0x00FF0000;
    entry->access = access;
    
    // 0 - 3: limit 16-19, 4 - 7: flags
    entry->flags = ((limit & 0x000F0000) >> 16) | ((flags & 0xF) << 4); 

    entry->base3 = base & 0xFF000000;
    
    return index; 
}