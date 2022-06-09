#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ubsan.h>
#include <math.h>
#include <common.h>
#include <array.h>
#include <arraylist.h>
#include <stream.h>
#include <time.h>
#include <hash.h>
#include <hashmap.h>
#include <gfx/color.h>
#include <gfx/font.h>
#include <gfx/image.h>
#include <gfx/primitives/vec2d.h>
#include <gfx/primitives/rectangle.h>
#include <core/system/multiboot.h>
#include <core/system/debug.h>
#include <core/system/tests.h>
#include <core/system/assets.h>
#include <core/memory/memmgr.h>
#include <core/memory/heap.h>
#include <core/memory/gc.h>
#include <core/exec/thread.h>
#include <core/exec/scheduler.h>
#include <core/exec/interpreter/tokenizer.h>
#include <hardware/device.h>
#include <hardware/devmgr.h>
#include <hardware/pci.h>
#include <hardware/devices/ata.h>
#include <hardware/devices/vbe.h>
#include <hardware/common/realmode.h>
#include <hardware/common/ports.h>
#include <hardware/common/serial.h>
#include <hardware/common/registers.h>
#include <hardware/common/rtc.h>
#include <hardware/common/pit.h>
#include <hardware/interrupts/gdt.h>
#include <hardware/interrupts/idt.h>
#include <hardware/interrupts/pic.h>
#include <filesys/filesys.h>
#include <filesys/types/ramfs.h>
#include <filesys/parsing/config_parser.h>
#include <filesys/parsing/bitmap.h>

namespace os
{
    typedef struct
    {
        std::ivec2d_t screen_size;
        bool has_parsed;
    } PACKED kernel_args_t;

    /// @brief Namespace containing object instances and methods for the kernel
    namespace kernel
    {
        /// @brief A copy of the multiboot header
        extern sys::multiboot_t multiboot;

        /// @brief Starting arguments specified from bootloader
        extern kernel_args_t start_args;

        /// @brief Kernel heap designated for large allocations - aligned to 4096 bytes
        extern memory_heap heap_large;

        /// @brief Kernel heap designated for small allocations - aligned to 256 bytes 
        extern memory_heap heap_small;

        /// @brief Kernel thread instance
        extern threading::thread_t* thread;
        
        /// @brief Boot sequence for kernel - initializes debugging, memory, threading, and devices
        void boot();
        
        /// @brief To be executed before entering kernel main, but after threading is enabled
        void before_main();

        /// @brief Main loop for kernel
        void main();

        /// @brief Parse kernel start arguments
        void parse_args();

        /// @brief Get starting address of kernel memory @return Address value
        uint32_t start_addr();

        /// @brief Get ending address of kernel memory @return Address value
        uint32_t end_addr();

        /// @brief Get bottom of kernel stack @return Address value
        uint32_t stk_start_addr();

        /// @brief Get Top of kernel stack @return Address value
        uint32_t stk_end_addr();

        /// @brief Get starting address of BSS section memory @return Address value
        uint32_t bss_start_addr();

        /// @brief Get ending address of BSS section memory @return Address value
        uint32_t bss_end_addr();
    }
}

EXTC
{
    void kernel_main(os::sys::multiboot_t* mboot);
}

