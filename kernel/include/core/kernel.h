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
#include <core/system/multiboot.h>
#include <core/system/debug.h>
#include <hardware/common/ports.h>
#include <hardware/common/serial.h>
#include <hardware/common/registers.h>
#include <hardware/interrupts/gdt.h>
#include <hardware/interrupts/idt.h>
#include <hardware/interrupts/pic.h>

namespace os
{
    namespace kernel
    {
        extern sys::multiboot_t       multiboot;
        
        void boot();
        void main();
    }
}