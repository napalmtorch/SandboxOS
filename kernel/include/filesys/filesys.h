#pragma once
#include <stdint.h>
#include <array.h>
#include <arraylist.h>
#include <common.h>

namespace os
{
    namespace filesystem { typedef struct filesys_t filesys_t; }

    enum class file_mode : uint8_t { read, read_write };
    typedef struct
    {
        char*                  name;
        std::array<uint8_t>   data;
        filesystem::filesys_t* fs;
        file_mode              mode;
    } PACKED file_t;

    typedef struct
    {
        char*    name;
        uint32_t file_count;
        uint32_t subdir_count;
    } PACKED directory_t;

    typedef struct
    {
        char*  name;
        size_t size;
    } PACKED fileinfo_t;

    namespace filesystem
    {
        enum class filesystem_type : uint8_t { ramfs, diskfs, fat32 };

        struct filesys_t
        {
            filesystem_type type;
            void*           host;
            char            letter;
            char            label[22];
        };

        void init();

        void       fs_register(filesys_t* fs);
        void       fs_unregister(filesys_t* fs);
        filesys_t* fs_create(filesystem_type type, char letter, char* label, void* host);
    
        filesys_t* from_path(const char* path);
        uint8_t    from_modestr(const char* mode);

        bool file_exists(const char* fname);
        bool dir_exists(const char* fname);

        file_t* open_file(const char* fname, const char* mode);
        size_t  read_file(void* buff, size_t size, size_t nmemb, file_t* file);
        size_t  write_file(const void* buff, size_t size, size_t nmemb, file_t* file);
        void    close_file(file_t* file);

        std::arraylist<fileinfo_t> fetch_files(const char* path);
        std::arraylist<char*> fetch_dirs(const char* path);
    }
}