#ifndef SERIAL_H
#define SERIAL_H

#include "io.h"

#define SERIAL_COM1 0x03F8
#define SERIAL_DATA_PORT(base)				(base)
#define SERIAL_ENABLE_INTERRUPT(base)		(base + 1)
#define SERIAL_FIFO_CONTROL(base)			(base + 2)
#define SERIAL_LINE_CNTROL(base)			(base + 3)
#define SERIAL_MODEM_CONTROL(base)			(base + 4)
#define SERIAL_LINE_STATUS(base)			(base + 5)
#define SERIAL_MODEM_STATUS(base)			(base + 6)
#define SERIAL_SCRATCH(base)				(base + 7)
#define SERIAL_DIVISOR_LSB(base)			(base)
#define SERIAL_DIVISOR_MSB(base)			(base + 1)

void serial_init_port(unsigned short com);
int serial_is_fifo_empty(unsigned short com);
void serial_write_ch(unsigned short com, unsigned char data);
void serial_write(unsigned short com, unsigned char * data, unsigned int length);
void serial_puts(unsigned short com, char * s);
void serial_put_hex(unsigned short com, unsigned int hex);
void serial_put_hex64(unsigned short com, unsigned long long hex);
void serial_put_dec(unsigned short com, unsigned int num);

#endif // SERIAL_H