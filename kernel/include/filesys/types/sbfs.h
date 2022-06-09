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
            uint32_t sector_count;
            uint16_t bytes_per_sector; 
            uint32_t blk_table_start;
            uint32_t blk_table_count;
            uint32_t blk_table_count_max;
            uint32_t blk_table_sector_count;
            uint32_t blk_data_start;
            uint32_t blk_data_sector_count;
            uint32_t blk_data_used;
            uint32_t file_table_start;
            uint32_t file_table_count;
            uint32_t file_table_count_max;
            uint32_t file_table_sector_count;
        } PACKED sbfs_info_t;

        typedef struct
        {
            uint32_t start;
            uint32_t count;
            uint16_t state;
            uint8_t  padding[6];
        } PACKED sbfs_blkentry_t;

        typedef struct
        {
            char     name[46];
            uint32_t parent_index;
            uint8_t  status;
            uint8_t  type;
            uint8_t  padding[12];
        } PACKED sbfs_directory_t;

        typedef struct
        {
            char     name[46];
            uint32_t parent_index;
            uint8_t  status;
            uint8_t  type;
            uint32_t size;
            uint32_t blk_index;
            uint8_t* data;
        } PACKED sbfs_file_t;

        class sbfs_host
        {
            public:
                void init();
                void info_read();
                void info_write();
                sbfs_info_t info();

            public:
                bool create_root(char* label);

            public:
                uint32_t            blktable_sector_from_index(int index);
                uint32_t            blktable_offset_from_index(uint32_t sector, int index);
                sbfs_blkentry_t     blktable_read(int index);
                void                blktable_write(int index, sbfs_blkentry_t entry);
                sbfs_blkentry_t     blktable_allocate(uint32_t sectors);
                bool                blktable_free(sbfs_blkentry_t entry);
                sbfs_blkentry_t     blktable_nearest(sbfs_blkentry_t entry);
                void                blktable_merge_free();
                bool                blktable_copy(sbfs_blkentry_t dest, sbfs_blkentry_t src);
                sbfs_blkentry_t     blktable_create_entry(uint32_t start, uint32_t count, uint8_t state);
                bool                blktable_delete_entry(sbfs_blkentry_t entry);
                bool                blktable_fill(sbfs_blkentry_t entry, uint8_t value);
                sbfs_blkentry_t     blktable_at_index(int index);
                bool                blktable_validate_sector(uint32_t sector);
                int                 blktable_get_index(sbfs_blkentry_t entry);
                int                 blktable_freeindex();

            public:
                uint32_t          filetable_sector_from_index(int index);
                uint32_t          filetable_offset_from_index(uint32_t sector, int index);
                sbfs_directory_t  filetable_read_dir(int index);
                sbfs_file_t       filetable_read_file(int index);
                void              filetable_write_dir(int index, sbfs_directory_t dir);
                void              filetable_write_file(int index, sbfs_file_t file);
                sbfs_directory_t  filetable_create_dir(sbfs_directory_t dir);
                sbfs_file_t       filetable_create_file(sbfs_file_t file);
                bool              filetable_delete_dir(sbfs_directory_t dir);
                bool              filetable_delete_file(sbfs_file_t file);
                bool              filetable_validate_sector(uint32_t sector);
                int               filetable_freeindex();

            public:
                bool              dir_equals(sbfs_directory_t a, sbfs_directory_t b);
                bool              file_equals(sbfs_file_t a, sbfs_file_t b);
                sbfs_directory_t  parent_from_path(const char* path);
                sbfs_file_t       get_file_byname(const char* path);
                sbfs_directory_t  get_dir_byname(const char* path);
                int               get_file_index(sbfs_file_t file);
                int               get_dir_index(sbfs_directory_t dir);
                char*             get_name_from_path(const char* path);
                char*             get_parent_path_from_path(const char* path);
                sbfs_file_t       file_create(const char* path, uint32_t size);
                sbfs_file_t       file_read(const char* path);
                bool              file_write(const char* path, uint8_t* data, uint32_t len);
        };
    }
}