#pragma once
#include <stdint.h>

EXTC
{
    extern void _write_cr0(uint32_t cr0);

    extern void _write_cr3(uint32_t cr3);

    extern void _write_cr4(uint32_t cr4);

    extern uint32_t _read_cr0();

    extern uint32_t _read_cr2();

    extern uint32_t _read_cr3();

    extern uint32_t _read_cr4();
}