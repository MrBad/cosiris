#include <stdio.h>
#include <stdarg.h>

int sprintf(char *str, const char *fmt, ...)
{
    va_list args;
    int ret;

    va_start(args, fmt);
    ret = vsprintf(str, fmt, args);
    va_end(args);

    return ret;
}

// this is hacky thing
int snprintf(char *str, size_t size, const char *fmt, ...)
{
    va_list args;
    int ret;

    va_start(args, fmt);
    ret = vsprintf(str, fmt, args);
    va_end(args);

    return ret;
}
