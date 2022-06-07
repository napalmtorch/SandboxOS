#pragma once
#include <stdint.h>
#include <stddef.h>

typedef uint16_t portnum_t;

namespace os
{
    namespace hal
    {
        namespace ports
        {
            static inline uint8_t inb(portnum_t port)
            {
                uint8_t result;
                asm volatile("in %%dx, %%al" : "=a" (result) : "d" (port));
                return result;
            }

            static inline uint16_t inw(portnum_t port)
            {
                uint16_t result;
                asm volatile("in %%dx, %%ax" : "=a" (result) : "d" (port));
                return result;
            }

            static inline uint32_t inl(portnum_t port)
            {
                uint32_t result;
                asm volatile("inl %%dx, %%eax" : "=a" (result) : "dN" (port));
                return result;
            }

            static inline void insw(portnum_t port, uint8_t* buff, size_t size) { asm volatile("rep insw" : "+D" (buff), "+c" (size) : "d" (port) : "memory"); }
            
            static inline void outb(portnum_t port, uint8_t data) { asm volatile("out %%al, %%dx" : : "a" (data), "d" (port)); }

            static inline void outw(portnum_t port, uint16_t data) { asm volatile("out %%ax, %%dx" : : "a" (data), "d" (port)); }

            static inline void outl(portnum_t port, uint32_t data) { asm volatile("outl %%eax, %%dx" : : "dN" (port), "a" (data)); }

            static inline void outsw(portnum_t port, uint8_t* data, size_t size) { asm volatile("rep outsw" : "+S" (data), "+c" (size) : "d" (port)); }
        }
    }
}