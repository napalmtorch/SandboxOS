#include <hardware/interrupts/gdt.h>
#include <core/kernel.h>

#define KERNEL_CS_INDEX 1
#define KERNEL_DS_INDEX 2

EXTC
{
    os::hal::gdtr_t      _gdt_reg                ALIGNED(0x100);
    os::hal::gdt_entry_t _gdt_entries[GDT_COUNT] ALIGNED(0x100);

    const os::hal::gdt_access_t KERNEL_CS_ACCESS = (os::hal::gdt_access_t)
    {
        .accessed        = 0,
        .read_write      = 1,
        .dc_flag         = GDT_NONCONF,
        .executable      = true,
        .type            = 1,
        .privilege_level = 0,
        .present         = 1,
    };

    const os::hal::gdt_access_t KERNEL_DS_ACCESS = (os::hal::gdt_access_t)
    {
        .accessed        = 0,
        .read_write      = 1,
        .dc_flag         = os::hal::GDTDIR_UPWARDS,
        .executable      = false,
        .type            = 1,
        .privilege_level = 0,
        .present         = 1,
    };

    const os::hal::gdt_flags_t KERNEL_CS_FLAGS = (os::hal::gdt_flags_t)
    {
        .reserved    = 0,
        .long_mode   = false,
        .size        = os::hal::GDTSIZE_32BIT,
        .granularity = os::hal::GDTGRAN_ALIGN_4K,
    };

    const os::hal::gdt_flags_t KERNEL_DS_FLAGS = (os::hal::gdt_flags_t)
    {
        .reserved    = 0,
        .long_mode   = false,
        .size        = os::hal::GDTSIZE_32BIT,
        .granularity = os::hal::GDTGRAN_ALIGN_4K,
    };

    extern void _gdt_flush(uint32_t gdt);
}

namespace os
{
    namespace hal
    {
        namespace gdt
        {
            void init()
            {
                memset(&_gdt_entries, 0, sizeof(gdt_entry_t) * GDT_COUNT);
    
                _gdt_reg.base  = (uint32_t)&_gdt_entries;
                _gdt_reg.limit = (GDT_COUNT * sizeof(gdt_entry_t)) - 1;

                printf("%s Creating GDT entries...\n", DEBUG_INFO);
                init_descriptors();

                _gdt_flush((uint32_t)&_gdt_reg);
                printf("%s Initialized GDT - ADDR:0x%8x\n", DEBUG_OK, (uint32_t)&_gdt_reg);
            }

            void init_descriptors()
            {
                set_descriptor(0, 0, 0, (gdt_access_t){ 0, 0, false, 0, 0, 0, 0 }, (gdt_flags_t){ 0, false, GDTSIZE_16BIT, GDTGRAN_ALIGN_1B });
                set_descriptor(KERNEL_CS_INDEX, 0, 0xFFFFFFFF, KERNEL_CS_ACCESS, KERNEL_CS_FLAGS);
                set_descriptor(KERNEL_DS_INDEX, 0, 0xFFFFFFFF, KERNEL_DS_ACCESS, KERNEL_DS_FLAGS);
            }

            void set_descriptor(uint8_t n, uint32_t base, uint32_t limit, gdt_access_t access, gdt_flags_t flags)
            {
                _gdt_entries[n].base_low = base & 0xFFFF;
                _gdt_entries[n].base_middle = (base >> 16) & 0xFF;
                _gdt_entries[n].base_high = (base >> 24) & 0xFF;
                
                _gdt_entries[n].limit_low = limit & 0xFFFF;
                _gdt_entries[n].limit_high = (limit >> 16) & 0x0F;
                
                _gdt_entries[n].accessed = access.accessed;
                _gdt_entries[n].read_write = access.read_write;
                _gdt_entries[n].dc_flag = access.dc_flag;
                _gdt_entries[n].executable = access.executable;
                _gdt_entries[n].type = access.type;
                _gdt_entries[n].privilege_level = access.privilege_level;
                _gdt_entries[n].present = access.present;
                
                _gdt_entries[n].reserved = 0;
                _gdt_entries[n].long_mode = flags.long_mode;
                _gdt_entries[n].size = flags.size;
                _gdt_entries[n].granularity = flags.granularity;

                printf("%s GDT[%d] - BASE: 0x%8x LIMIT: 0x%8x ACCESS: 0x%2x FLAGS: 0x%2x\n", DEBUG_INFO, n, base, limit, access, flags);
            }
        }
    }
}