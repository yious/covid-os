#ifndef GDT_H
#define GDT_H

struct gdt_entry
{
    unsigned short limit; // bits 0 - 15
    unsigned short base; // bits 16 - 31
    unsigned char base2; // bits 32 - 39
    unsigned char access; // 40 - 47
    unsigned char flags; // 48 - 55 (limit2 + flags)
    unsigned char base3; // 56 - 63
} __attribute__((packed));

struct gdt {
    unsigned short size;
    struct gdt_entry * address;
} __attribute__((packed));

int gdt_load_entry(
    unsigned short index,
    unsigned int limit,
    unsigned int base,
    unsigned char access,
    unsigned char flags
);

extern void lgdt(struct gdt * gdt);

extern void flush_selectors();

void gdt_init();

#endif // GDT_H