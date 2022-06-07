#pragma once
#include <stdint.h>
#include <array.h>
#include <filesys/filesys.h>

namespace os
{
    namespace filesystem
    {
        typedef struct
        {
            char     name[32];
            uint8_t* data;
            uint32_t size;
        } PACKED ramfs_file_t;

        typedef struct
        {
            uint32_t      count;
            uint32_t      count_max;
            uint32_t      cur_address;
            ramfs_file_t* files;
        } PACKED ramfs_info_t;

        class ramfs_host
        {
            private:
                std::array<uint8_t> _data;
                ramfs_info_t*        _info;

            public:
                void    init(std::array<uint8_t> data);
                bool    exists(const char* fname);
                file_t* read(const char* fname);

            public:
                ramfs_info_t* info();

        };
    }
}