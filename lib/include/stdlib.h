#pragma once
#include <stdint.h>
#include <stddef.h>

EXTC
{
    #define MEMTYPE_FREE   0
    #define MEMTYPE_OBJ    1
    #define MEMTYPE_ARRAY  2
    #define MEMTYPE_STRING 3

    void*    malloc(size_t size);
    void*    tmalloc(size_t size, uint8_t type);
    void     free(void* ptr);
    void     freearray(void** ptr, uint32_t count);
    int      atoi(const char* str);
    uint32_t atol(const char* str);
    uint32_t atox(const char* str);
    float    atof(const char* str);
    int      rand();
    void     exit(int code);
    char*    itoa(int num, char* str, int base);
    char*    ltoa(size_t num, char* str, int base);
    char*    strhex(uint32_t value, char* result, bool prefix, uint8_t bytes);
    int      system(char* command);
    char*    strerror(int errnum);
    void     perror(const char* str, ...);
}