#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <gfx/image.h>

namespace os
{
    namespace filesystem
    {
        namespace parsing
        {
            typedef struct
            {
                uint16_t type;
                uint32_t size;
                uint16_t reserved0;
                uint16_t reserved1;
                uint32_t off_bits;
            } PACKED bmp_fileheader_t;

            typedef struct
            {
                uint32_t size;
                uint32_t width;
                uint32_t height;
                uint16_t planes;
                uint16_t bit_count;
                uint32_t compression;
                uint32_t size_image;
                uint32_t x_ppm;
                uint32_t y_ppm;
                uint32_t clr_used;
                uint32_t clr_important;
            } bmp_infoheader_t;

            std::gfx::image bmp_load(char* fname);
        }
    }
}