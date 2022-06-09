#pragma once
#include <stdint.h>
#include <stddef.h>
#include <gfx/color.h>
#include <gfx/image.h>
#include <gfx/font.h>
#include <core/system/multiboot.h>
#include <hardware/device.h>

namespace os
{
    namespace hal
    {
        /// @brief VBE graphics controller interface
        class vbe_controller : public device_t
        {
            private:
                sys::vbe_ctrl_info_t _ctrlinfo;
                sys::vbe_mode_info_t _modeinfo;
                size_t               _size;

            public:
                /// @brief Initialize ATA controller           
                void init() override;
                /// @brief Start ATA controller driver
                void start() override;
                /// @brief Stop ATA controller driver
                void stop() override;

            private:
                /// @brief Populate information headers with data from multiboot
                void populate_headers();

            public:
                /// @brief Set current resolution of display @param w Desired width @param h Desired height
                void set_mode(int w, int h);
                /// @brief Clear he display @param color 32-bit color value
                void clear(uint32_t color = 0xFF000000);
                /// @brief Draw colored pixel at desired position @param x X coordinate @param y Y coordinate @param color Color of pixel
                void blit(int x, int y, uint32_t color = 0xFF000000);
                /// @brief Draw filled rectangle at desired position @param x X coordinate @param y Y coordinate @param w Width of rectangle @param h Height of rectangle @param color Color of rectangle
                void rect_filled(int x, int y, int w, int h, uint32_t color = 0xFF000000);
                /// @brief Put ascii character at desired position @param x X coordinate @param y Y coordinate @param c Character value @param font Bitfont @param fg Foreground color @param bg Background color
                void putc(int x, int y, char c, std::gfx::bitfont font, uint32_t fg, uint32_t bg = 0x00FFFFFF);
                /// @brief Put ascii string at desired position @param x X coordinate @param y Y coordinate @param str Pointer to string @param font Bitfont @param fg Foreground color @param bg Background color
                void putstr(int x, int y, char* str, std::gfx::bitfont font, uint32_t fg, uint32_t bg = 0x00FFFFFF);
                /// @brief Copy array of pixels to display @param x X coordinate @param y Y coordinate @param w Width of source data @param h Height of source data @param data Source data
                void copy(int x, int y, int w, int h, uint32_t* data);

            public:
                /// @brief Get color of pixel at specified position @param x X coordinate @param y Y coordinate @return 32-bit color value
                uint32_t get_pixel(int x, int y);

            public:
                /// @brief Get VBE mode information @return VBE mode info structure
                sys::vbe_mode_info_t modeinfo();
            
        };

        namespace devices
        {
            extern vbe_controller* vbe;
        }
    }
}