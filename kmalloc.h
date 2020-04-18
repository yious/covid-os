#ifndef KMALLOC_H
#define KMALLOC_H

extern unsigned int _kernel_virtual_end;

unsigned int kmalloc_a(unsigned int size);  // page aligned.
unsigned int kmalloc_p(unsigned int size, unsigned int *paddr); // returns a paddrical address.
unsigned int kmalloc_ap(unsigned int size, unsigned int *paddr); // page aligned and returns a physical address.
unsigned int kmalloc(unsigned int size); // vanilla (normal).

#endif // KMALLOC_H