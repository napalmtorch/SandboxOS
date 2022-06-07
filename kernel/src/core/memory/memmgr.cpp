#include <core/memory/memmgr.h>
#include <core/kernel.h>

#define MEMBLK_COUNT 1024

namespace os
{
    /// @brief Toggle debug messages
    bool memory_manager::messages;
    
    /// @brief Array of memory block entries
    memblk_t  memory_manager::_memblks[MEMBLK_COUNT];

    /// @brief Free memory block pointer
    memblk_t* memory_manager::_freeblk;

    /// @brief Pointer to memory block directly after free block
    memblk_t* memory_manager::_after_freeblk;

    /// @brief Amount of usable memory after initializing
    size_t memory_manager::_usable;

    /// @brief Initialize physical memory manager
    void memory_manager::init()
    {
        memset(&_memblks, 0, sizeof(memblk_t) * MEMBLK_COUNT);
        _freeblk       = NULL;
        _after_freeblk = NULL;
        _usable        = 0;
        messages       = false;
        
        probe_mmap();
        probe_mods();

        printf("%s Zeroing free memory from 0x%8x to 0x%8x\n", DEBUG_INFO, _freeblk->address, _freeblk->address + _freeblk->size);
        memset((void*)_freeblk->address, 0, _freeblk->size);
        _usable = _freeblk->size;

        messages = true;
        print();
        printf("%s Initialized memory manager\n", DEBUG_OK);
    }

    /// @brief Read memory map and create corresponding blocks
    void memory_manager::probe_mmap()
    {
        sys::mmap_entry_t* mmap    = (sys::mmap_entry_t*)kernel::multiboot.mmap_addr;
        size_t             count   = kernel::multiboot.mmap_len / sizeof(sys::mmap_entry_t);

        for (size_t i = 0; i < count; i++)
        {
            if (mmap[i].type == 0) { continue; }
            if (mmap[i].address_low < kernel::start_addr() && mmap[i].type == (uint32_t)memblk_type::available) { continue; }
            memblk_t* blk = map(mmap[i].address_low, mmap[i].length_low, (memblk_type)mmap[i].type);
            if (mmap[i].address_low > kernel::start_addr() && mmap[i].type == (uint32_t)memblk_type::reserved && _after_freeblk == NULL) { _after_freeblk = blk; }
            if (blk->address == kernel::start_addr()) { _freeblk = blk; }
        }

        if (_freeblk == NULL) { perror("Failed to parse memory map\n"); }
        _freeblk->address += (kernel::end_addr() - kernel::start_addr());
        _freeblk->size    -= (kernel::end_addr() - kernel::start_addr());

        map(kernel::start_addr(), kernel::end_addr() - kernel::start_addr(), memblk_type::kernel);
    }

    /// @brief Read modules and create corresponding blocks
    void memory_manager::probe_mods()
    {
        sys::multiboot_module_t* mods = (sys::multiboot_module_t*)kernel::multiboot.modules_addr;
        if (mods == NULL) { return; }

        uint32_t mods_end  = 0;
        uint32_t mods_size = 0;
        for (size_t i = 0; i < kernel::multiboot.modules_count; i++)
        {
            if (mods[i].address_start == 0) { continue; }
            uint32_t end = memalign(mods[i].address_end, 0x1000);
            map(mods[i].address_start, end - mods[i].address_start, memblk_type::module);
            mods_end = end;
            mods_size += (end - mods[i].address_start);
        }

        _freeblk->address  = mods_end;
        _freeblk->size     = (_after_freeblk->address - _freeblk->address); // + (kernel::end_addr() - kernel::start_addr());
    }

    /// @brief Create memory block and add to list @param addr Memory address @param size Memory size @param type Type of memory @return Pointer to memory block
    memblk_t* memory_manager::map(uint32_t addr, size_t size, memblk_type type)
    {
        if (size == 0 || type == memblk_type::invalid) { perror("Tried to map invalid memory block - ADDR:0x%8x TYPE:0x%2x SIZE:%d bytes", addr, type, size); return NULL; }

        memblk_t* blk = next();
        if (blk == NULL) { perror("Maximum amount of memory blocks reached"); return NULL; }
        *blk = (memblk_t){ .address = addr, .size = size, .type = type };
        if (messages) { printf("%s TYPE:%s ADDR:0x%8x SIZE:%u bytes\n", DEBUG_PMAP, typestr(type), addr, size); }
        return blk;
    }

    /// @brief Request a memory block of specified size @param size Requested size @param type Type of memory @return Pointer to memory block
    memblk_t* memory_manager::request(size_t size, memblk_type type)
    {
        if (size == 0) { perror("Attempt to request 0 bytes of physical memory"); return NULL; }
        if (type == memblk_type::invalid || type == memblk_type::bad_ram) { perror("Attempt to request physical memory of invalid type"); return NULL; }

        for (size_t i = 0; i < MEMBLK_COUNT; i++)
        {
            if (_memblks[i].size >= size && _memblks[i].type == memblk_type::available)
            {
                memblk_t* blk = map(_memblks[i].address, size, type);
                _memblks[i].address += size;
                _memblks[i].size    -= size;
                return blk;
            }
        }

        perror("Failed to request %u bytes of physical memory", size);
        return NULL;
    }

    /// @brief Get next available block entry in list @return Pointer to unused memory block
    memblk_t* memory_manager::next()
    {
        for (size_t i = 0; i < MEMBLK_COUNT; i++)
        {
            if (_memblks[i].type == memblk_type::invalid) { return &_memblks[i]; }
        }
        return NULL;
    }

    /// @brief Get total amount of usable memory detected after memory manager initialization - value will no longer be valid after requests are made @return Amount of usable memory in bytes
    size_t memory_manager::usable() { return _usable; }
    
    /// @brief Convert memory block type into string representation @param type Type of memory @return Pointer to string representation
    const char* memory_manager::typestr(memblk_type type)
    {
        switch (type)
        {
            default:                            { return "invalid     "; }
            case memblk_type::available:        { return "available   "; }
            case memblk_type::reserved:         { return "reserved    "; }
            case memblk_type::acpi_reclaimable: { return "acpi reclaim"; }
            case memblk_type::nvs:              { return "nvs         "; }
            case memblk_type::bad_ram:          { return "bad ram     "; }
            case memblk_type::kernel:           { return "kernel      "; }
            case memblk_type::heap:             { return "heap        "; }
            case memblk_type::module:           { return "module      "; }
        }
    }

    void memory_manager::print()
    {
        printf("%s Showing list of memory blocks\n", DEBUG_INFO);
        for (size_t i = 0; i < MEMBLK_COUNT; i++)
        {
            if (_memblks[i].type == memblk_type::invalid) { continue; }
            printf("%s ADDR:0x%8x TYPE:%s SIZE:%u bytes\n", DEBUG_MMAP, _memblks[i].address, typestr(_memblks[i].type), _memblks[i].size);
        }
    }
}