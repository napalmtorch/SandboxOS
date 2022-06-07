#include <core/exec/thread.h>
#include <core/kernel.h>

EXTC
{
    os::threading::thread_t* THREAD;
    os::threading::thread_t* THREAD_NEXT;
}

namespace os
{
    namespace threading
    {
        uint32_t _mon_now, _mon_last;

        /// @brief Set thread name @param thread Pointer to thread @param str Pointer to name string
        void thread_setname(thread_t* thread, const char* str)
        {
            if (thread == NULL) { return; }
            if (str == NULL || strlen(str) == 0) { return; }
            if (thread->name != NULL) { free(thread->name); }
            thread->name = (char*)tmalloc(strlen(str) + 1, ALLOCTYPE_STRING);
            strcpy(thread->name, str);
        }

        /// @brief Called when thread returns from entry point
        void thread_exit()
        {
            asm volatile("cli");
            register int eax asm("eax");
            int exit_code = eax;
            THREAD->flags.terminated = true;
            printf("Thread %u('%s') exited with code %u\n", THREAD->id, THREAD->name, exit_code);
            asm volatile("sti");
            while (true) { scheduler::yield(); }
        }

        /// @brief Create a new thread instance @param name Name of thread @param stacksz Size of stack in bytes @param protocol Entry point of thread @param argc Arguments count @param argv Pointer to argument list @return Pointer to thread
        thread_t* thread_create(char* name, size_t stacksz, thread_protocol_t protocol, int argc, char** argv)
        {
            thread_t* t = (thread_t*)tmalloc(sizeof(thread_t), ALLOCTYPE_OBJ);
            thread_setname(t, name);
            t->id        = scheduler::generate_id();
            t->arguments = std::array<char*>(argv, argc);
            t->stack     = std::array<uint8_t>(stacksz < 0x1000 ? 0x1000 : stacksz);
            t->protocol  = protocol;

            uint32_t* stk = (uint32_t*)((uint32_t)t->stack.ptr() + (t->stack.length() - sizeof(thread_regs_t) - 20));
            *--stk = (uint32_t)t;
            *--stk = (uint32_t)argv;
            *--stk = (uint32_t)argc;
            *--stk = (uint32_t)thread_exit;
            *--stk = (uint32_t)protocol;

            t->registers.edx    = (uint32_t)t;
            t->registers.esp    = (uint32_t)stk;
            t->registers.eflags = (uint32_t)0x202;
            printf("%s ADDR:0x%8x STACK:0x%8x-0x%8x PROTOCOL:0x%8x ARGC:%u ID:%u NAME:%s\n", DEBUG_THREAD, t, t->stack.ptr(), t->stack.ptr() + t->stack.length(), protocol, argc, t->id, t->name);
            return t;
        }

        /// @brief Create kernel thread instance @return Kernel thread
        thread_t* thread_create_k()
        {
            thread_t* t = (thread_t*)tmalloc(sizeof(thread_t), ALLOCTYPE_OBJ);
            thread_setname(t, "kernel");
            t->id            = scheduler::generate_id();
            t->protocol      = (thread_protocol_t)kernel_main;
            t->stack         = std::array<uint8_t>((uint8_t*)kernel::stk_start_addr(), kernel::stk_end_addr() - kernel::stk_start_addr());
            t->flags.running = true;
            printf("%s ADDR:0x%8x STACK:0x%8x-0x%8x PROTOCOL:0x%8x ARGC:%u ID:%u NAME:%s\n", DEBUG_THREAD, t, t->stack.ptr(), t->stack.ptr() + t->stack.length(), t->protocol, t->arguments.length(), t->id, t->name);
            return t;
        }

        /// @brief Dispose thread and corresponding allocations @param thread Pointer to thread @return Success of disposal
        bool thread_dispose(thread_t* thread)
        {
            if (thread == NULL) { return false; }
            thread->arguments.dispose();
            if (thread->stack.ptr() != NULL) { thread->stack.dispose(); }
            if (thread->name != NULL) { free(thread->name); }
            return true;
        }

        /// @brief Monitor and update performance of current thread
        void thread_monitor()
        {
            THREAD->ticks++;
        }
    }
}