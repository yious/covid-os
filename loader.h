#ifndef LOADER_H
#define LOADER_H

struct kernel_memory_info
{
unsigned int _kernel_virtual_start;
unsigned int _kernel_virtual_end;
unsigned int _kernel_physical_start;
unsigned int _kernel_physical_end;
}__attribute__((packed));
typedef struct kernel_memory_info kernel_memory_info_t;

#endif  //LOADER_H