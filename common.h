#ifndef COMMON_H
#define COMMON_H

#define UNUSED(x) (void)(x)

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

#define IS_BIT_SET(src, offset) ((src) & 1 << offset) 

unsigned int physical_to_virtual(unsigned int paddr);
void panic(char *message, char *file, unsigned int line);

#endif // COMMON_H