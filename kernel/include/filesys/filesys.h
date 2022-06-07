#pragma once
#include <stdint.h>
#include <array.h>
#include <arraylist.h>
#include <common.h>

namespace os
{
    namespace filesystem { typedef struct filesys_t filesys_t; }

    /// @brief File access modes
    enum class file_mode : uint8_t 
    { 
        /// @brief Read-only file system
        read, 
        /// @brief Read-write file system
        read_write 
    };

    /// @brief Structure for managing files
    typedef struct
    {
        /// @brief Filename pointer - allocated on heap
        char* name;
        /// @brief File data
        std::array<uint8_t> data;
        /// @brief Pointer to parent file system
        filesystem::filesys_t* fs;
        /// @brief File mode
        file_mode mode;
    } PACKED file_t;

    /// @brief Structure for managing directories
    typedef struct
    {
        /// @brief Directory name - allocated on heap
        char* name;
        /// @brief Amount of files in directory
        uint32_t file_count;
        /// @brief Amount of sub-directories in directory
        uint32_t subdir_count;
    } PACKED directory_t;

    /// @brief Structure containing information about a file
    typedef struct
    {
        /// @brief Filename pointer - allocated on heap
        char*  name;
        /// @brief Size of file in bytes
        size_t size;
    } PACKED fileinfo_t;

    namespace filesystem
    {
        /// @brief File system types
        enum class filesystem_type : uint8_t 
        { 
            /// @brief Custom RAMFS
            ramfs, 
            /// @brief Custom DISKFS
            diskfs, 
            /// @brief USTAR file system
            ustar,
            /// @brief FAT12/16/13/EX file system
            fat32 
        };

        /// @brief Structure for managing virtual file systems
        struct filesys_t
        {
            /// @brief File system type
            filesystem_type type;
            /// @brief Pointer to file system controller
            void* host;
            /// @brief Drive letter
            char letter;
            /// @brief Drive label - maximum of 22 bytes
            char label[22];
        };

        /// @brief Initialize virtual file system serivce
        void init();

        /// @brief Register virtual file system @param fs Pointer to file system
        void fs_register(filesys_t* fs);

        /// @brief Un-register virtual file system @param fs Pointer to file system
        void fs_unregister(filesys_t* fs);

        /// @brief Create new virtual file system instance @param type File systme type @param letter Drive letter @param label Drive label - maximum of 22 bytes @param host Pointer to fs controller @return Pointer to virtual file system
        filesys_t* fs_create(filesystem_type type, char letter, char* label, void* host);
    
        /// @brief Get file system from full path @param path Pointer to path string @return Pointer to virtual file systrm
        filesys_t* from_path(const char* path);

        /// @brief Convert mode string to numeral representation @param mode Pointer to mode string @return File mode
        uint8_t from_modestr(const char* mode);

        /// @brief Check if file exists @param fname Pointer to filename string @return File was located
        bool file_exists(const char* fname);

        /// @brief Check if directory exists @param fname Pointer to path string @return Directory was located
        bool dir_exists(const char* path);

        /// @brief Open specified file @param fname Pointer to filename string @param mode Pointer to file mode string /// @return Pointer to file - returns null if file was not located
        file_t* open_file(const char* fname, const char* mode); 

        /// @brief Read data from file @param buff Pointer to destination @param size Size of elements @param nmemb Element count @param file Pointer to file @return Number of bytes read
        size_t read_file(void* buff, size_t size, size_t nmemb, file_t* file);

        /// @brief Write data to file @param buff Pointer to source @param size Size of elements @param nmemb Element size, @return Number of bytes written
        size_t write_file(const void* buff, size_t size, size_t nmemb, file_t* file);

        /// @brief Close opened file @param Pointer to file
        void close_file(file_t* file);

        /// @brief Get list of files in directory @param path Pointer to path string @return List of files
        std::arraylist<fileinfo_t> fetch_files(const char* path);

        /// @brief Get list of sub-directories in directory @param path Pointer to path string @return List of directories
        std::arraylist<char*> fetch_dirs(const char* path);
    }
}