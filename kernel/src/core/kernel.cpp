#include <core/kernel.h>

namespace os
{
    namespace kernel
    {
        /// @brief Multiboot information header
        sys::multiboot_t multiboot;

        /// @brief Boot sequence for kernel
        void boot()
        {
            // initialize serial port
            sys::debug::init(hal::serial_port::com1);

            // print multiboot information
            sys::multiboot_print(&multiboot);

            // clear vram to confirm boot
            unsigned int* vram = (unsigned int*)0xFD000000;
            for (int i = 0; i < 640 * 480; i++) { vram[i] = 0xFF007F7F; }
        }

        /// @brief Main loop for kernel
        void main()
        {
            printf("%s Entered kernel main\n", DEBUG_OK);
            while (true)
            {

            }
        }
    }
}

/// @brief Kernel entry point from assembly @param mboot Multiboot header pointer
extern "C" void kernel_main(os::sys::multiboot_t* mboot)
{
    os::kernel::multiboot = *mboot;
    os::kernel::boot();
    os::kernel::main();
}