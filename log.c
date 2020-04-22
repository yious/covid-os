#include "log.h"
#include "serial.h"
#include "common.h"
#include <stdarg.h>

void log_init()
{
    return;
}

void log(char * severity, char * frmt, va_list vl)
{
    char ch = 0;
    BOOL is_special_mode = FALSE;

    serial_puts(SERIAL_COM1, severity);
    serial_puts(SERIAL_COM1, ": ");
    for (char * ptr = frmt; *ptr != '\0' ; ptr++)
    {
        ch = *ptr;
        if (is_special_mode)
        {
            // last char was '%'
            switch (ch)
            {
            case 's':
            {
                char * st = va_arg(vl, char *);
                serial_puts(SERIAL_COM1, st);
                break;
            }
            case '%':
            {
                serial_write_ch(SERIAL_COM1, '%');
                break;
            }
            case 'c':
            {
                char st = (char)va_arg(vl, int);
                serial_write_ch(SERIAL_COM1, st);
                break;
            }
            case 'u':
            {
                unsigned int num = va_arg(vl, unsigned int);
                serial_put_dec(SERIAL_COM1, num);
                break;
            }
            case 'p':
            case 'x':
            {
                const unsigned int num = va_arg(vl, const unsigned int);
                serial_put_hex(SERIAL_COM1, num);
                break;
            }

            default:
                log_error("Invalid string specifier '%c'\n", ch);
                break;
            }
            is_special_mode = FALSE; 
        }
        else 
        {
            if (ch == '%')
            {
                is_special_mode = TRUE;
            }
            else
            {
                serial_write_ch(SERIAL_COM1, *ptr);
            }
        }
    }
}
void log_info(char * frmt, ...)
{
    va_list vl;
    va_start(vl, frmt);
    log("INFO", frmt, vl);
    va_end(vl); 
}

void log_error(char * frmt, ...)
{
    va_list vl;
    va_start(vl, frmt);
    log("ERROR", frmt, vl);
    va_end(vl); 
}