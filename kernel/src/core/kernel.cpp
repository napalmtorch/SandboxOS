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
        sys::multiboot_t     multiboot;
        kernel_args_t        start_args;
        memory_heap          heap_small;
        memory_heap          heap_large;
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

            // parse kernel start arguments
            start_args.has_parsed = false;
            parse_args();

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
            file_t* font_file = filesystem::open_file("A:/font.bin", "r");
            if (font_file != NULL)
            {
                uint8_t* data = new uint8_t[font_file->data.length()];
                memcpy(data, font_file->data.ptr(), font_file->data.length());
                filesystem::close_file(font_file);
                std::FONT_DEFAULT = std::gfx::bitfont(8, 12, 1, 0, data);
                printf("%s Loaded primary font from 'A:/font.bin'\n", DEBUG_INFO);
            }
            else { std::FONT_DEFAULT = std::gfx::bitfont(8, 14, 1, 0, (uint8_t*)std::FONTDATA_DEFAULT); }
        }

        /// @internal NICO - this is probably where you wanna test the interpreter, as there is no garbage collection in the boot function xDDD
        void before_main()
        {
            
        }

        void main()
        {
            lock();
            printf("%s Entered kernel main\n", DEBUG_OK);

            std::gfx::image logo = std::gfx::image("A:/logo.bmp");

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
                    //printf("TM:%s, RUNTIME:%u, MEM:%u/%u KB, THREADS: %u\n", std::timestr(tm, tmstr, std::time_format::standard, true), seconds, memused / KB, memtotal / KB, threading::scheduler::threads()->length());
                    //printf("KERNEL - CPU USAGE: %u%% MEMORY USAGE: %u bytes\n", kernel::thread->time.cpu_usage, heap_small.calc_used(kernel::thread) + heap_large.calc_used(kernel::thread));
                    //printf("GC     - CPU USAGE: %u%% MEMORY USAGE: %u bytes\n", garbage_collector::thread->time.cpu_usage, heap_small.calc_used(garbage_collector::thread) + heap_large.calc_used(garbage_collector::thread));
                    //printf("--------------------------------------------------------------------------------------------------------\n");
                    hal::devices::vbe->clear(0xFF000000);
                    if (logo.data().ptr() != NULL) { hal::devices::vbe->copy((hal::devices::vbe->modeinfo().width / 2) - (logo.size().x / 2), (hal::devices::vbe->modeinfo().height / 2) - (logo.size().y / 2), logo.size().x, logo.size().y, logo.data().ptr()); }
                    sys::debug::draw_overlay();
                }
                threading::scheduler::monitor();

                int r = rand(255);
                printf("RAND: %d\n", r);

                unlock();
                threading::scheduler::yield();
            }
        }

        void parse_args()
        {
            // set default start arguments
            if (!start_args.has_parsed)
            {
                start_args.screen_size = std::ivec2d_t(640, 480);
            }

            char* argstr = (char*)multiboot.command_line;
            if (argstr == NULL || strlen(argstr) == 0) { printf("%s No kernel arguments specified\n", DEBUG_WARN); return; }
            int args_count = 0;
            char** args = strsplit(argstr, ' ', &args_count);

            int pos = 0;
            while (true)
            {
                if (pos >= args_count) { break; }

                // screen width
                if (!strcmp(args[pos], "-w"))
                {
                    if (pos + 1 >= args_count) { printf("%s Error parsing width value in kernel arguments\n", DEBUG_ERROR); break; }
                    start_args.screen_size.x = atol(args[pos + 1]);
                    pos += 2;
                }
                // screen height
                else if (!strcmp(args[pos], "-h"))
                {
                    if (pos + 1 >= args_count) { printf("%s Error parsing height value in kernel arguments\n", DEBUG_ERROR); break; }
                    start_args.screen_size.y = atol(args[pos + 1]);
                    pos += 2;
                }
                else { pos++; }
            }
            freearray((void**)args, args_count);
            start_args.has_parsed = true;
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