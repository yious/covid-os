#ifndef IO_H
#define IO_H

void __attribute__((__cdecl__)) outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);


#endif // IO_H