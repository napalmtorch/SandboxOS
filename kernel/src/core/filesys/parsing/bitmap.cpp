#include <filesys/parsing/bitmap.h>
#include <core/kernel.h>

namespace os
{
    namespace filesystem
    {
        namespace parsing
        {
            std::gfx::image bmp_load(char* fname)
            {
                file_t* file = filesystem::open_file(fname, "r");
                if (file == NULL) { printf("%s Unable to locate bitmap at '%s'\n", DEBUG_ERROR, fname); return std::gfx::image(); }

                bmp_fileheader_t* h = (bmp_fileheader_t*)file->data.ptr();
                bmp_infoheader_t* info = (bmp_infoheader_t*)((uint32_t)file->data.ptr() + sizeof(bmp_fileheader_t));
                if (info->width == 0 || info->height == 0) { printf("%s Invalid width and height for bitmap at '%s'\n", DEBUG_ERROR, fname); close_file(file); return std::gfx::image(); }

                std::gfx::image img(info->width, info->height);
                uint8_t* img_data = (uint8_t*)((uint32_t)file->data.ptr() + h->off_bits);
                uint32_t size = img.size().x * img.size().y * 4;

                for (int yy = img.size().y - 1; yy >= 0; yy--)
                {
                    for (int xx = 0; xx < img.size().x; xx++)
                    {
                        if (info->bit_count == 24)
                        {
                            uint32_t o    = (3 * (yy * img.size().x + xx));
                            uint32_t c    = (0xFF << 24) | (img_data[o + 2] << 16) | (img_data[o + 1] << 8) | img_data[o];
                            uint32_t dest = xx + ((img.size().y - yy - 1) * img.size().x);
                            img.data().ptr()[dest] = c;

                        }
                        else if (info->bit_count == 32)
                        {
                            uint32_t o    = (4* (yy * img.size().x + xx));
                            uint32_t c    = (0xFF << 24) | (img_data[o + 2] << 16) | (img_data[o + 1] << 8) | img_data[o];
                            uint32_t dest = xx + ((img.size().y - yy - 1) * img.size().x);
                            img.data().ptr()[dest] = c;
                        }
                    }
                }
                printf("%s Successfully loaded bitmap at '%s' - W:%u H:%u SIZE:%u bytes\n", DEBUG_OK, fname, info->width, info->height, size);
                close_file(file);
                return img;
            }
        }
    }
}