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
            bool mmdbg = false;
            memory_heap::init(mmdbg);

            // initialize thread scheduler
            threading::scheduler::init();

            // initialize device manager
            hal::device_manager::init();

            // start garbage collector thread
            garbage_collector::start();
            garbage_collector::messages = true;

            // initialize filesystem
            filesystem::init();

            // initialize default font
            std::FONT_DEFAULT = std::gfx::bitfont(8, 14, 1, 0, (uint8_t*)std::FONTDATA_DEFAULT);
        }

        /// @internal NICO - this is probably where you wanna test the interpreter, as there is no garbage collection in the boot function xDDD
        void before_main()
        {
            
        }

        void main()
        {
            lock();
            printf("%s Entered kernel main\n", DEBUG_OK);
            unlock();

            uint32_t seconds = 0, now = 0, last = 0;
            char tmstr[32];
            while (true)
            {                
                lock();
                threading::thread_monitor();
                std::time_t tm = std::timenow();
                now = tm.second;
                if (now != last)
                { 
                    // reset values and increment second counter
                    last = now; 
                    seconds++; 
                    memset(tmstr, 0, sizeof(tmstr));
                    
                    // calculate memory usage
                    uint32_t memused  = heap_large.calc_used() + heap_small.calc_used();
                    uint32_t memtotal = heap_large.data_size() + heap_small.data_size();

                    // print info
                    printf("TM:%s, RUNTIME:%u, MEM:%u/%u KB, THREADS: %u\n", std::timestr(tm, tmstr, std::time_format::standard, true), seconds, memused / KB, memtotal / KB, threading::scheduler::threads()->length());
                    printf("KERNEL_CPU:%u%%(%u ticks) GC_CPU:%u%%(%u ticks)\n\n", kernel::thread->time.cpu_usage, kernel::thread->time.tps, garbage_collector::thread->time.cpu_usage, garbage_collector::thread->time.tps);
                }
                threading::scheduler::monitor();

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
        os::kernel::before_main();
        os::kernel::main();
    }
}