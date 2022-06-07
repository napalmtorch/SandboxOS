#pragma once
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

EXTC
{
    void vprintf(const char* fmt, va_list args);
    void printf(const char* fmt, ...);
}