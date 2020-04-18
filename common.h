#ifndef COMMON_H
#define COMMON_H

#define UNUSED(x) (void)(x)

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

#define IS_BIT_SET(src, offset) ((src) & 1 << offset) 

#define NULL    0
#define BOOL    int
#define TRUE    1
#define FALSE   0

unsigned int physical_to_virtual(unsigned int paddr);
void * memset ( void * ptr, int value, unsigned int num );
void panic(const char *message, char *file, unsigned int line);

#endif // COMMON_H