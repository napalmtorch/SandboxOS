#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <array.h>

#define STACKSZ_SMALL  (8   * KB)
#define STACKSZ_MEDIUM (64  * KB)
#define STACKSZ_LARGE  (512 * KB)

namespace os
{
    namespace threading
    {
        typedef struct
        {
            bool running    : 1;
            bool locked     : 1;
            bool terminated : 1;
            bool sleeping   : 1;
        } PACKED thread_flags_t;

        typedef struct
        {
            uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
            uint32_t eflags;
        } PACKED thread_regs_t;

        typedef int (*thread_protocol_t)(int, char**);

        typedef struct
        {
            thread_regs_t       registers;
            thread_flags_t      flags;
            thread_protocol_t   protocol;
            std::array<char*>   arguments;
            std::array<uint8_t> stack;
            uint8_t             cpu_usage;
            uint32_t            sleeptime;
            uint32_t            ticks, fracticks;
            uint32_t            tps;
            uint32_t            id;
            char*               name;
        } PACKED thread_t;

        thread_t* thread_create(char* name, size_t stacksz, thread_protocol_t protocol, int argc, char** argv);
        thread_t* thread_create_k();
        bool      thread_dispose(thread_t* thread);
        void      thread_monitor();
    }
}

EXTC
{
    extern os::threading::thread_t* THREAD;
    extern os::threading::thread_t* THREAD_NEXT;
}