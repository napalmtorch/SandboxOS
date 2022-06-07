#pragma once
#include <stdint.h>
#include <array.h>
#include <filesys/filesys.h>

namespace os
{
    namespace filesystem
    {
        /// @brief Structure for managing RAMFS files
        typedef struct
        {
            /// @brief Filename - maximum of 32 bytes
            char name[32];
            /// @brief File data
            uint8_t* data;
            /// @brief Size of data in bytes
            uint32_t size;
        } PACKED ramfs_file_t;

        /// @brief Structure for managing RAMFS information header
        typedef struct
        {
            /// @brief File count
            uint32_t      count;
            /// @brief Maximum amount of files
            uint32_t      count_max;
            /// @brief Last used address
            uint32_t      cur_address;
            /// @brief Pointer to file list
            ramfs_file_t* files;
        } PACKED ramfs_info_t;

        /// @brief RAM file system manager
        class ramfs_host
        {
            private:
                /// @brief File system data
                std::array<uint8_t> _data;
                /// @brief File system info header
                ramfs_info_t* _info;

            public:
                /// @brief Initialized RAMFS @param data Pointer to file system data
                void init(std::array<uint8_t> data);
                /// @brief Check if file exists @param fname Pointer to filename string @return File was located
                bool exists(const char* fname);
                /// @brief Read specified file @param fname Pointer to filename string @return Pointer to file - returns null if file was not located
                file_t* read(const char* fname);

            public:
                /// @brief Get pointer to file system info header
                ramfs_info_t* info();

        };
    }
}