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
        /// @brief Multiboot information header
        sys::multiboot_t multiboot;

        /// @brief Kernel heap used for small allocations aligned to 256 bytes
        memory_heap heap_small;

        /// @brief Kernel heap used for larger allocations aligned to 4096 bytes
        memory_heap heap_large;

        /// @brief Kernel thread instance
        threading::thread_t* thread;

        /// @brief Boot sequence for kernel
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

            // initialize pit
            hal::pit::init();

            // start garbage collector thread
            garbage_collector::start();
            garbage_collector::messages = false;

            // initialize filesystem
            filesystem::init();
        }

        /// @brief Main loop for kernel
        void main()
        {
            lock();
            printf("%s Entered kernel main\n", DEBUG_OK);

            uint32_t memused  = heap_large.calc_used() + heap_small.calc_used();
            uint32_t memtotal = heap_large.data_size() + heap_small.data_size();
            printf("%s Memory usage: %u/%u bytes(%u/%u MB)\n", DEBUG_INFO, memused, memtotal, memused / MB, memtotal / MB);
            unlock();

            while (true)
            {
                lock();
                void* test = tmalloc(1024, ALLOCTYPE_ARRAY);
                free(test);
                unlock();
                threading::scheduler::yield();
            }
        }

        /// @brief Return starting address of kernel memory
        uint32_t start_addr() { return (uint32_t)&_kernel_start; }

        /// @brief Return ending address of kernel memory
        uint32_t end_addr() { return (uint32_t)&_kernel_end; }
        
        /// @brief Return address to bottom of stack
        uint32_t stk_start_addr() { return (uint32_t)&_stack_bottom; }

        /// @brief Return address to top of stack
        uint32_t stk_end_addr() { return (uint32_t)&_stack_top; }

        /// @brief Return staring address of BSS section
        uint32_t bss_start_addr() { return (uint32_t)&_bss_start; }

        /// @brief Return ending address of BSS section
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