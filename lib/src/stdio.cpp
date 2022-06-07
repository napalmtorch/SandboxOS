#include <stdio.h>
#include <stdlib.h>
#include <core/kernel.h>

#ifdef __cplusplus
EXTC
{
#endif

void vprintf(const char* fmt, va_list args)
{
    if (os::sys::debug::stdout_enabled) { os::sys::debug::serial.vprint_fmt((char*)fmt, args); }
}

void printf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

#ifdef __cplusplus
}
#endif