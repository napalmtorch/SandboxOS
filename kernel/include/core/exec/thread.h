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
        /// @brief Structure for managing thread flags
        typedef struct
        {
            /// @brief Is thread running
            bool running    : 1;
            /// @brief Is thread locked
            bool locked     : 1;
            /// @brief Is thread terminated
            bool terminated : 1;
            /// @brief Is thread sleeping
            bool sleeping   : 1;
        } PACKED thread_flags_t;

        /// @brief Structure for managing cpu registers for thread contexts
        typedef struct
        {
            uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
            uint32_t eflags;
        } PACKED thread_regs_t;

        /// @brief Structure for managing thread performance and time information
        typedef struct
        {
            uint32_t ticks;
            uint32_t tpsf;
            uint32_t tps;
            uint8_t  cpu_usage;
        } PACKED thread_time_t;

        /// @brief Thread protocol @param argc Arguments count @param argv Pointer to arguments @return Exit code
        typedef int (*thread_protocol_t)(int argc, char** argv);

        /// @brief Structure for managing thread context and properties
        typedef struct
        {
            /// @brief CPU registers structure
            thread_regs_t registers;
            /// @brief Thread flags structure
            thread_flags_t flags;
            /// @brief Thread entry point
            thread_protocol_t protocol;
            /// @brief Thread arguments
            std::array<char*> arguments;
            /// @brief Thread stack
            std::array<uint8_t> stack;
            /// @brief Thread performance and time information
            thread_time_t time;
            /// @brief Thread identification number
            uint32_t id;
            /// @brief Pointer to thread name
            char* name;
        } PACKED thread_t;

        /// @brief Create a new thread instance @param name Pointer to name @param stacksz Size of stack in bytes @param protocol Pointer to entry function @param argc Arguments count @param argv Pointer to arguments @return Pointer to thread
        thread_t* thread_create(char* name, size_t stacksz, thread_protocol_t protocol, int argc, char** argv);

        /// @brief Create the kernel thread instance - registers are automatically populated on first context switch @return Pointer to thread
        thread_t* thread_create_k();

        /// @brief Free memory allocations for specified thread @param thread Pointer to thread @return Thread disposed successfully
        bool thread_dispose(thread_t* thread);

        /// @brief Monitor and update thread performance and time information
        void thread_monitor();
    }
}

EXTC
{
    /// @brief Currently active thread
    extern os::threading::thread_t* THREAD;

    /// @brief Next thread in queue - only accurate within the context of os::threading::scheduler::yield
    extern os::threading::thread_t* THREAD_NEXT;
}