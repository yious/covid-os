#ifndef LOG_H
#define LOG_H

#include <stdarg.h>

void log_init();

void log(char * severity, char * frmt, va_list vl);
void log_info(char * frmt, ...);
void log_error(char * frmt, ...);

#endif // LOG_H