#ifndef MALLOC_H
#define MALLOC_H

typedef long Align;

struct header
{
    struct header * next;
    unsigned int size;
};
typedef struct header header_t;

void * kmalloc(unsigned int size);
void * kcalloc(unsigned int size);
void kfree(void * ptr);

#endif // MALLOC_H