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
#include <core/system/multiboot.h>
#include <core/system/debug.h>
#include <core/system/tests.h>
#include <core/memory/memmgr.h>
#include <core/memory/heap.h>
#include <core/memory/gc.h>
#include <core/exec/thread.h>
#include <core/exec/scheduler.h>
#include <hardware/common/ports.h>
#include <hardware/common/serial.h>
#include <hardware/common/registers.h>
#include <hardware/common/pit.h>
#include <hardware/interrupts/gdt.h>
#include <hardware/interrupts/idt.h>
#include <hardware/interrupts/pic.h>
#include <filesys/ramfs.h>
#include <filesys/filesys.h>

namespace os
{
    namespace kernel
    {
        extern sys::multiboot_t     multiboot;
        extern memory_heap          heap_large;
        extern memory_heap          heap_small;
        extern threading::thread_t* thread;
        
        void boot();
        void main();

        uint32_t start_addr();
        uint32_t end_addr();
        uint32_t stk_start_addr();
        uint32_t stk_end_addr();
        uint32_t bss_start_addr();
        uint32_t bss_end_addr();
    }
}

EXTC
{
    void kernel_main(os::sys::multiboot_t* mboot);
}