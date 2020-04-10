#include "multiboot.h"

struct mmap_entry
{
    unsigned int size;
    unsigned long long base;
    unsigned long long length; 
    unsigned int type;
}__attribute__((packed));

typedef struct mmap_entry mmap_entry_t;

extern unsigned int _kernel_physical_end; 

void pfa_init(multiboot_info_t * mbinfo);
void pfa_setup(unsigned int * storage, unsigned int len);

unsigned int pfa_alloc(unsigned int size);
void pfa_free(unsigned int vaddr);