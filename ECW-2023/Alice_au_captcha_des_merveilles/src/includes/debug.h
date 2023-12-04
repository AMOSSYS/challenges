#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdarg.h>

void debug_vprintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

#define DEBUG 0

#define var_debug(x) do { if (DEBUG) { debug_vprintf ("%s:%d:%s(): ", \
    __FILE__,  __LINE__, __func__); debug_vprintf x; }} while (0)

/* var_dump("%s" variable_name); */
#define var_dump(fmt, var) do { if (DEBUG) { debug_vprintf ("%s:%d:%s(): ", \
    __FILE__,  __LINE__, __func__); debug_vprintf ("%s = " fmt, #var, var); }} while (0)

#endif
