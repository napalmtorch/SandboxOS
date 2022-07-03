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
        sys::multiboot_t      multiboot;
        kernel_args_t         start_args;
        memory_heap           heap_small;
        memory_heap           heap_large;
        threading::thread_t*  thread;
        services::shell_host* shell;

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

            // load assets
            sys::asset_manager::init();

            // initialize command handler
            sys::command_handler::init();

            // start shell
            shell = new services::shell_host();
            shell->start();
        }

        /// @internal NICO - this is probably where you wanna test the interpreter, as there is no garbage collection in the boot function xDDD
        void before_main()
        {
            //lock();
            //filesystem::parsing::ini_parser ini("A:/boot.ini");
            //ini.run();
            //unlock();
        }

        void main()
        {
            lock();
            printf("%s Entered kernel main\n", DEBUG_OK);
            unlock();

            while (true)
            {
                lock();
                threading::thread_monitor();
                threading::scheduler::monitor();
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