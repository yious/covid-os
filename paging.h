#ifndef PAGING_H
#define PAGING_H

#define CR0_WP_ENABLE 0x1 << 16
#define CR0_PG_ENABLE 0x1 << 31

#define CR4_PSE_ENABLE 0x1 << 4
#define CR4_PAE_ENABLE 0x1 << 5
#define CR4_PGE_ENABLE 0x1 << 7
#define CR4_PCIDE_ENABLE 0x1 << 17
#define CR4_SMEP_ENABLE 0x1 << 20
#define CR4_SMAP_ENABLE 0x1 << 21
#define CR4_PKE_ENABLE 0x1 << 22

#define PAGING_PRESENT 0x1
#define PAGING_RW 0x2
#define PAGING_PRIV 0x4
#define PAGING_PWT 0x8
#define PAGING_PCD 0x10
#define PAGING_ACCESS 0x20
#define PAGING_DIRTY 0x40
#define PAGING_PAGE_SIZE 0x80

#define PAGING_PRESENT 0x1
#define PAGING_NOT_PRESENT 0x0
#define PAGING_RW 0x2
#define PAGING_RO 0x0
#define PAGING_PRIVLEGE_USER 0x4
#define PAGING_PRIVLEGE_SUPERVISOR 0x0
#define PAGING_PWT_ENABLE 0x8
#define PAGING_PWT_DISABLE 0x0
#define PAGING_PCD_ENABLE 0x10
#define PAGING_PCD_DISABLE 0x0
#define PAGING_PAGE_SIZE_4MB 0x80
#define PAGING_PAGE_SIZE_4KB 0x0

#define IDENTITY_PAGING_PDE_SIZE 1024


struct pde {
    unsigned char flags;
    unsigned char middle; // only 1 lsb and 4 msb used
    unsigned short high_address;
} __attribute__((packed));
typedef struct pde pde_t; 

struct pte {
    unsigned char flags;
    unsigned char low_address; //only  4 msb used
    unsigned short high_address;
} __attribute__((packed));
typedef struct pde pde_t; 

void paging_init();

extern void paging_enable(pde_t * base);

extern void pde_start(void);

#endif // PAGING_H