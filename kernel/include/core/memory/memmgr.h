#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>

namespace os
{
    enum class memblk_type : uint8_t
    {
        invalid,
        available,
        reserved,
        acpi_reclaimable,
        nvs,
        bad_ram,
        kernel,
        module,
        heap,
    };

    typedef struct
    {
        uint32_t    address;
        size_t      size;
        memblk_type type;
    } PACKED memblk_t;

    class memory_manager
    {
        public:
            static bool messages;

        private:
            static memblk_t  _memblks[];
            static memblk_t* _freeblk;
            static memblk_t* _after_freeblk;
            static size_t    _usable;

        public:
            static void init();
            static void print();
            static void probe_mmap();
            static void probe_mods();

        public:
            static memblk_t* map(uint32_t addr, size_t size, memblk_type type);
            static memblk_t* request(size_t size, memblk_type type = memblk_type::reserved);
            static memblk_t* first_bytype(memblk_type type);

        public:
            static size_t      usable();
            static const char* typestr(memblk_type type);
            

        private:
            static memblk_t* next();
            
    };
}