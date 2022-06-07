#pragma once
#include <stdint.h>

EXTC
{
    /// @brief Write value to CR0 register @param cr0 Specified value
    extern void _write_cr0(uint32_t cr0);

    /// @brief Write value to CR3 register @param cr3 Specified value
    extern void _write_cr3(uint32_t cr3);

    /// @brief Write value to CR4 register @param cr4 Specified value
    extern void _write_cr4(uint32_t cr4);

    /// @brief Read value from CR0 register @return Value of CR0
    extern uint32_t _read_cr0();

    /// @brief Read value from CR2 register @return Value of CR2
    extern uint32_t _read_cr2();

    /// @brief Read value from CR3 register @return Value of CR3
    extern uint32_t _read_cr3();

    /// @brief Read value from CR4 register @return Value of CR4
    extern uint32_t _read_cr4();
}