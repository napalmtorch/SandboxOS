#include <filesys/filesys.h>
#include <core/kernel.h>

namespace os
{
    namespace filesystem
    {
        std::arraylist<filesys_t*> filesystems;

        void init()
        {
            // fetch ramfs module
            memblk_t* blk = memory_manager::first_bytype(memblk_type::module);
            if (blk == NULL) { printf("%s No RAMFS detected\n", DEBUG_WARN); return; }

            // initialize ramfs
            ramfs_host* rfsh  = (ramfs_host*)tmalloc(sizeof(ramfs_host), ALLOCTYPE_OBJ);
            filesys_t*  ramfs = fs_create(filesystem_type::ramfs, 'A', "RAMDISK", rfsh);
            rfsh->init(std::array<uint8_t>((uint8_t*)blk->address, blk->size));
            fs_register(ramfs);

            printf("%s Initialized filesystem manager\n", DEBUG_OK);
        }

        void fs_register(filesys_t* fs)
        {
            if (fs == NULL) { perror("Tried to register null filesystem"); return; }
            filesystems.add(fs);
            printf("%s Register filesystem - TYPE:0x%2x HOST:0x%8x LETTER:%c LABEL:%s\n", DEBUG_INFO, fs->type, fs->host, fs->letter, fs->label);
        }
        
        void fs_unregister(filesys_t* fs)
        {
            if (fs == NULL) { perror("Tried to unregister null filesystem"); return; }
            for (size_t i = 0; i < filesystems.length(); i++)
            {
                if (filesystems[i] == fs)
                {
                    printf("%s Unregistered filesystem - TYPE:0x%2x HOST:0x%8x LETTER:%c LABEL:%s\n", DEBUG_INFO, fs->type, fs->host, fs->letter, fs->label);
                    filesystems.remove_at(i);
                    return;
                }
            }
            perror("Failed to unregister filesystem at 0x%8x", fs);
        }
        
        filesys_t* fs_create(filesystem_type type, char letter, char* label, void* host)
        {
            filesys_t* fs = (filesys_t*)tmalloc(sizeof(filesys_t), ALLOCTYPE_OBJ);
            fs->type   = type;
            fs->letter = letter;
            fs->host   = host;
            for (size_t i = 0; i < strlen(label); i++) { if (i < sizeof(fs->label) - 1) { fs->label[i] = label[i]; } }
            return fs;
        }
    
        filesys_t* from_path(const char* path)
        {
            if (strlen(path) < 3) { return NULL; }
            if (path[1] != ':' || path[2] != '/') { return NULL; }

            for (size_t i = 0; i < filesystems.length(); i++)
            {
                if (filesystems[i]->letter == path[0]) { return filesystems[i]; }
            }
            return NULL;
        }
        
        uint8_t from_modestr(const char* mode)
        {
            if (!strcmp(mode, "w")) { return 1; }
            return 0;            
        }

        bool file_exists(const char* fname)
        {
            filesys_t* fs = from_path(fname);
            if (fs == NULL) { return false; }

            if (fs->type == filesystem_type::ramfs) { return ((ramfs_host*)fs->host)->exists(fname + 3); }
            return false;
        }
        
        bool dir_exists(const char* path)
        {
            filesys_t* fs = from_path(path);
            if (fs == NULL) { return false; }

            if (fs->type == filesystem_type::ramfs)
            {
                if (path[0] == fs->letter && path[1] == ':' && path[2] == '/' && strlen(path) == 3) { return true; }
                return false;
            }
            return false;
        }

        file_t* open_file(const char* fname, const char* mode)
        {
            filesys_t* fs = from_path(fname);
            if (fs == NULL) { return NULL; }

            file_mode m = (file_mode)from_modestr(mode);

            if (fs->type == filesystem_type::ramfs)
            {
                if (m == file_mode::read_write) { perror("Cannot open file for reading on read-only filesystem"); return NULL; }
                file_t* fptr = ((ramfs_host*)fs->host)->read(fname + 3);
                if (fptr == NULL) { return NULL; }
                fptr->fs = fs;
                return fptr;
            }
            return NULL;
        }
        
        size_t read_file(void* buff, size_t size, size_t nmemb, file_t* file)
        {
            if (file == NULL) { perror("Attempt to read from null file pointer"); return 0; }
            if (buff == NULL) { perror("Attempt to read file '%s' into null buffer", file->name); return 0; }
            if (nmemb == 0 || size == 0) { return 0; }

            size_t success = 0;
            for (size_t i = 0; i < size; i++)
            {
                void* res = memcpy(buff + (i * nmemb), file->data.ptr() + (i * nmemb), nmemb);
                if (res == NULL) { return 0; }
                success++;
            }
            return success;
        }
        
        size_t write_file(const void* buff, size_t size, size_t nmemb, file_t* file)
        {
            if (file == NULL) { perror("Attempt to write to null file pointer"); return 0; }
            if (file->fs->type == filesystem_type::ramfs) { perror("Cannot write file to read-only filesystem at 0x%8x", file->fs); return 0; }
        }

        void close_file(file_t* file)
        {
            if (file == NULL) { return; }
            if (file->name != NULL) { free(file->name); }
            if (file->data.ptr() != NULL) { free(file->data.ptr()); }
            free(file);
        }

        std::arraylist<fileinfo_t> fetch_files(const char* path)
        {
            std::arraylist<fileinfo_t> files;

            filesys_t* fs = from_path(path);
            if (fs == NULL) { return files; }

            if (fs->type == filesystem_type::ramfs)
            {
                if (strlen(path) > 3) { return files; }
                ramfs_host* ramfs = ((ramfs_host*)fs->host);
                for (size_t i = 0; i < ramfs->info()->count_max; i++)
                {
                    if (ramfs->info()->files[i].size == 0) { continue; }
                    char* name = (char*)tmalloc(strlen(ramfs->info()->files[i].name) + 1, ALLOCTYPE_ARRAY);
                    strcpy(name, ramfs->info()->files[i].name);
                    files.add((fileinfo_t){ .name = name, .size = ramfs->info()->files[i].size });
                }
            }
            return files;
        }
        
        std::arraylist<char*> fetch_dirs(const char* path)
        {
            std::arraylist<char*> dirs;

            return dirs;
        }
    }
}