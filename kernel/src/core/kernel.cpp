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
    int test_main(int argc, char** argv)
    {
        printf("%s Entered test thread\n", DEBUG_INFO);
        return 420;
    }

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
            bool mmdbg = true;
            uint32_t usable = memalign(memory_manager::usable() - (16 * MB), 0x1000);
            heap_large.init((usable / 5) * 3, 16384, 0x1000, false);
            heap_small.init((usable / 5) * 2, 32768, 0x100, false);
            sys::tests::test_heap(10);
            heap_large.toggle_msgs(mmdbg);
            heap_small.toggle_msgs(mmdbg);

            // initialize thread scheduler
            threading::scheduler::init();

            // initialize filesystem
            filesystem::init();

            // open file
            const char* fname = "A:/test.txt";
            file_t* file = filesystem::open_file(fname, "r");
            if (file == NULL) { printf("%s Failed to open file '%s'\n", DEBUG_ERROR, fname); }
            else
            {
                printf("%s\n", file->data);
                filesystem::close_file(file);
            }

            // create test thread
            threading::thread_t* t = threading::thread_create("test", STACKSZ_MEDIUM, test_main, 0, NULL);
            threading::scheduler::load(t);
            threading::scheduler::start(t);

            // framebuffer test
            sys::vbe_mode_info_t* vbe = (sys::vbe_mode_info_t*)multiboot.vbe_mode_info;
            std::array<uint32_t> buff(vbe->width * vbe->height);
            memset(buff.ptr(), 0xFFFF7F1F, vbe->width * vbe->height * 4);
            memcpy((void*)vbe->physical_base, buff.ptr(), vbe->width * vbe->height * 4);
            buff.dispose();
        }

        /// @brief Main loop for kernel
        void main()
        {
            printf("%s Entered kernel main\n", DEBUG_OK);
            uint32_t memused = heap_large.calc_used() + heap_small.calc_used();
            printf("%s Memory used after boot: %u bytes(%d MB)\n", DEBUG_INFO, memused, memused / MB);

            while (true)
            {
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

/// @brief Kernel entry point from assembly @param mboot Multiboot header pointer
EXTC void kernel_main(os::sys::multiboot_t* mboot)
{
    os::kernel::multiboot = *mboot;
    os::kernel::boot();
    
    os::threading::scheduler::ready();
    os::kernel::main();
}