#include <filesys/ramfs.h>
#include <core/kernel.h>

namespace os
{
    namespace filesystem
    {
        void ramfs_host::init(std::array<uint8_t> data)
        {
            this->_data        = data;
            this->_info        = (ramfs_info_t*)this->_data.ptr();
            this->_info->files = (ramfs_file_t*)((uint32_t)this->_data.ptr() + sizeof(ramfs_info_t));
            printf("%s Initialized RAMFS - ADDR:0x%8x SIZE:%u MB\n", DEBUG_OK, this->_data.ptr(), this->_data.length() / MB);
        }

        bool ramfs_host::exists(const char* fname)
        {
            if (fname == NULL || strlen(fname) == 0) { return false; }
            for (size_t i = 0; i < this->_info->count_max; i++)
            {
                if (this->_info->files[i].size == 0 || this->_info->files[i].data == NULL) { continue; }
                if (!strcmp(this->_info->files[i].name, fname)) { return true; }
            }
            return false;
        }

        file_t* ramfs_host::read(const char* fname)
        {
            if (fname == NULL) { return NULL; }

            for (uint32_t i = 0; i < this->_info->count_max; i++)
            {
                if (this->_info->files[i].size == 0) { continue; }
                if (this->_info->files[i].data == NULL) { continue; }
                if (!strcmp(this->_info->files[i].name, fname)) 
                { 
                    file_t* output = (file_t*)tmalloc(sizeof(file_t), ALLOCTYPE_OBJ);
                    output->name = (char*)tmalloc(strlen(this->_info->files[i].name) + 1, ALLOCTYPE_ARRAY);
                    output->data = std::array<uint8_t>(this->_info->files[i].size);
                    strcpy(output->name, this->_info->files[i].name);
                    memcpy(output->data.ptr(), (void*)((uint32_t)this->_data.ptr() + this->_info->files[i].data), this->_info->files[i].size);
                    return output;
                }
            }
            return NULL;
        }

        ramfs_info_t* ramfs_host::info() { return _info; }
    }
}