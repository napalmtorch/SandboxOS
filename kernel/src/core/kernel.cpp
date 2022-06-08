#include <core/kernel.h>

EXTC
{
    extern uint32_t _kernel_start;
    extern uint32_t _kernel_end;
    extern uint32_t _bss_start;
    extern uint32_t _bss_end;
    extern uint32_t _stack_bottom;
    extern uint32_t _stack_top;
}

namespace os
{
    namespace kernel
    {
        sys::multiboot_t multiboot;
        memory_heap heap_small;
        memory_heap heap_large;
        threading::thread_t* thread;

        void boot()
        {
            // initialize serial port
            sys::debug::init(hal::serial_port::com1);

            // print multiboot information
            sys::multiboot_print(&multiboot);

            // initialize global descriptor table
            hal::gdt::init();
            hal::idt::init();

            // initialize physical memory manager
            memory_manager::init();

            // initialize heaps
            bool mmdbg = true;
            memory_heap::init(mmdbg);

            // initialize thread scheduler
            threading::scheduler::init();

            // initialize device manager
            hal::device_manager::init();

            // start garbage collector thread
            garbage_collector::start();
            garbage_collector::messages = false;

            // initialize filesystem
            filesystem::init();
        }

        void main()
        {
            lock();
            printf("%s Entered kernel main\n", DEBUG_OK);

            uint32_t memused  = heap_large.calc_used() + heap_small.calc_used();
            uint32_t memtotal = heap_large.data_size() + heap_small.data_size();
            printf("%s Memory usage: %u/%u bytes(%u/%u MB)\n", DEBUG_INFO, memused, memtotal, memused / MB, memtotal / MB);
            unlock();

            uint32_t seconds = 0, now = 0, last = 0;
            while (true)
            {
                lock();

                now = hal::pit::seconds();
                if (now != last) 
                { 
                    last = now; seconds++; 
                    uint32_t memused  = heap_large.calc_used() + heap_small.calc_used();
                    printf("SECONDS:%u, MEM:%u/%u bytes, THREADS: %u\n", seconds, memused, memtotal, threading::scheduler::threads()->length());
                }

                unlock();
                threading::scheduler::yield();
            }
        }

        uint32_t start_addr() { return (uint32_t)&_kernel_start; }

        uint32_t end_addr() { return (uint32_t)&_kernel_end; }
        
        uint32_t stk_start_addr() { return (uint32_t)&_stack_bottom; }

        uint32_t stk_end_addr() { return (uint32_t)&_stack_top; }

        uint32_t bss_start_addr() { return (uint32_t)&_bss_start; }

        uint32_t bss_end_addr() { return (uint32_t)&_bss_end; }
    }
}

EXTC
{
    void kernel_main(os::sys::multiboot_t* mboot)
    {
        os::kernel::multiboot = *mboot;
        os::kernel::boot();
        
        os::threading::scheduler::ready();
        os::kernel::main();
    }
}