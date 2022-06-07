#pragma once
#include <stdint.h>

EXTC
{
    /// @brief Write value to CR0 register @param cr0 CR0 value
    extern void _write_cr0(uint32_t cr0);

    /// @brief Write value to CR3 register @param cr0 CR3 value
    extern void _write_cr3(uint32_t cr3);

    /// @brief Write value to CR4 register @param cr0 CR4 value
    extern void _write_cr4(uint32_t cr4);

    /// @brief Read value from CR0 register @return CR0 value
    extern uint32_t _read_cr0();

    /// @brief Read value from CR0 register @return CR2 value
    extern uint32_t _read_cr2();

    /// @brief Read value from CR0 register @return CR3 value
    extern uint32_t _read_cr3();

    /// @brief Read value from CR0 register @return CR4 value
    extern uint32_t _read_cr4();
}