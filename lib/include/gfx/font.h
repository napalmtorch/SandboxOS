#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

namespace std
{
    namespace gfx
    {
        /// @brief Structure for managing bitfonts
        class bitfont
        {
            private:
                /// @brief Width of character
                uint8_t _cw;
                /// @brief Height of character
                uint8_t _ch;
                /// @brief Horizontal spacing of each character
                uint8_t _sx;
                /// @brief Vertical spacing for each character
                uint8_t _sy;
                /// @brief Pointer to physical font data
                uint8_t* _data;

            public:
                /// @brief Default font constructor - initializes all values to null
                bitfont() { _cw = 0; _ch = 0; _sx = 0; _sy = 0; _data = NULL; }
                /// @brief Create new bitfont structure with specified values @param cw Width of character @param ch Height of character @param sx Horizontal spacing of each character @param sy Vertical spacing of each character @param data Pointer to physical font data
                bitfont(uint8_t cw, uint8_t ch, uint8_t sx, uint8_t sy, uint8_t* data) { _cw = cw; _ch = ch; _sx = sx; _sy = sy; _data = data; }

            public:
                /// @brief Get pointer to physical font data
                uint8_t* data() { return _data; }
                /// @brief Get width of character @param spacing Add horizontal spacing to output value @return Width of character
                int width(bool spacing = true) { if (spacing) { return _cw + _sx; } return _cw; }
                /// @brief Get height of character @param spacing Add Vertical spacing to output value @return Height of character
                int height(bool spacing = true) { if (spacing) { return _ch + _sy; } return _ch; }
                /// @brief Get horizontal spacing of each character
                int spacing_x() { return _sx; }
                /// @brief Get vertical spacing of each character
                int spacing_y() { return _sy; }
        };
    }

    /// @brief Physical data for default font(8x14)
    static const uint8_t FONTDATA_DEFAULT[] = 
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x1C, 0x22, 0x41, 0x55, 0x41, 0x55, 0x5D, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x3E, 
        0x7F, 0x6B, 0x7F, 0x5D, 0x63, 0x3E, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x77, 0x7F, 0x7F, 
        0x3E, 0x3E, 0x1C, 0x1C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x1C, 0x3E, 0x7F, 0x3E, 
        0x1C, 0x1C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x1C, 0x08, 0x7F, 0x77, 0x2A, 0x08, 
        0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x3E, 0x7F, 0x7F, 0x77, 0x2A, 0x08, 0x1C, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x1C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xC3, 0xC3, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
        0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xE7, 
        0xDB, 0xBD, 0xBD, 0xBD, 0xDB, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x07, 0x03, 0x05, 
        0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x22, 0x22, 0x1C, 0x08, 
        0x08, 0x1C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0E, 0x0A, 0x0A, 0x08, 0x08, 0x18, 0x38, 
        0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1C, 0x16, 0x33, 0x71, 0x61, 0x03, 0x07, 0x06, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x08, 0x2A, 0x1C, 0x22, 0x63, 0x22, 0x1C, 0x2A, 0x08, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x60, 0x7C, 0x7F, 0x7C, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x03, 0x1F, 0x7F, 0x1F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 
        0x3E, 0x08, 0x08, 0x3E, 0x1C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 
        0x22, 0x22, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x45, 0x45, 0x45, 0x3D, 0x05, 
        0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x20, 0x18, 0x14, 0x0C, 0x02, 0x22, 
        0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x3E, 0x08, 0x08, 0x3E, 0x1C, 0x08, 0x00, 0x7F, 0x00, 0x00, 
        0x00, 0x00, 0x08, 0x1C, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x1C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x04, 0x06, 0x7F, 0x06, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x30, 
        0x7F, 0x30, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 
        0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x36, 0x7F, 0x36, 0x14, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x1C, 0x3E, 0x3E, 0x7F, 0x7F, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x3E, 0x3E, 0x1C, 0x1C, 0x08, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 
        0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0xFF, 0x24, 
        0x24, 0x24, 0xFF, 0x24, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x3E, 0x49, 0x48, 0x3E, 0x09, 
        0x49, 0x3E, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x51, 0x52, 0x24, 0x08, 0x12, 0x25, 0x45, 
        0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x24, 0x18, 0x28, 0x45, 0x42, 0x42, 0x3D, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x04, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x10, 0x08, 0x04, 0x04, 0x04, 0x04, 0x04, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x14, 0x08, 0x3E, 0x08, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 
        0x7F, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x18, 0x18, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x3C, 0x42, 0x46, 0x4A, 0x52, 0x62, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x08, 0x18, 0x28, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 
        0x02, 0x04, 0x18, 0x20, 0x40, 0x40, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x02, 0x02, 
        0x1C, 0x02, 0x02, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x0A, 0x12, 0x22, 0x42, 
        0x7F, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x40, 0x40, 0x40, 0x7C, 0x02, 0x02, 0x42, 
        0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x40, 0x40, 0x7C, 0x42, 0x42, 0x42, 0x3C, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x7E, 0x02, 0x02, 0x04, 0x08, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x3C, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x3C, 0x42, 0x42, 0x42, 0x3E, 0x02, 0x02, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 
        0x18, 0x00, 0x00, 0x18, 0x18, 0x08, 0x10, 0x00, 0x00, 0x00, 0x04, 0x08, 0x10, 0x20, 0x40, 0x20, 
        0x10, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x7F, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x3E, 0x41, 0x01, 0x01, 0x06, 0x08, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x1E, 0x21, 0x4D, 0x53, 0x53, 0x4D, 0x40, 0x21, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x08, 0x14, 0x22, 0x41, 0x41, 0x7F, 0x41, 0x41, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x21, 
        0x21, 0x21, 0x3E, 0x21, 0x21, 0x21, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x21, 0x40, 0x40, 
        0x40, 0x40, 0x40, 0x21, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x22, 0x21, 0x21, 0x21, 0x21, 
        0x21, 0x22, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x40, 0x40, 0x40, 0x7C, 0x40, 0x40, 0x40, 
        0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x40, 0x40, 0x40, 0x7C, 0x40, 0x40, 0x40, 0x40, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x1E, 0x21, 0x40, 0x40, 0x4F, 0x41, 0x41, 0x21, 0x1E, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x41, 0x41, 0x41, 0x41, 0x7F, 0x41, 0x41, 0x41, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x02, 
        0x02, 0x02, 0x02, 0x02, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x42, 0x44, 0x48, 
        0x70, 0x48, 0x44, 0x42, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
        0x40, 0x40, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x63, 0x55, 0x49, 0x49, 0x41, 0x41, 0x41, 
        0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x41, 0x61, 0x51, 0x49, 0x45, 0x43, 0x41, 0x41, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x7E, 0x41, 0x41, 0x41, 0x7E, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x1C, 0x22, 0x41, 0x41, 0x41, 0x49, 0x45, 0x22, 0x1D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x41, 
        0x41, 0x41, 0x7E, 0x48, 0x44, 0x42, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x41, 0x40, 0x40, 
        0x3E, 0x01, 0x01, 0x41, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x08, 0x08, 0x08, 0x08, 0x08, 
        0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 
        0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x41, 0x41, 0x22, 0x22, 0x14, 0x14, 0x08, 0x08, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x41, 0x41, 0x41, 0x41, 0x49, 0x49, 0x49, 0x55, 0x22, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x41, 0x41, 0x22, 0x14, 0x08, 0x14, 0x22, 0x41, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x41, 0x41, 0x22, 0x14, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x01, 
        0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x10, 0x10, 0x10, 
        0x10, 0x10, 0x10, 0x10, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 
        0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 
        0x3C, 0x00, 0x00, 0x00, 0x00, 0x08, 0x14, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 
        0x00, 0x00, 0x08, 0x10, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x3C, 0x02, 0x3E, 0x42, 0x42, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 
        0x40, 0x5C, 0x62, 0x42, 0x42, 0x62, 0x5C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 
        0x42, 0x40, 0x40, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x3A, 0x46, 0x42, 
        0x42, 0x46, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x7E, 0x40, 0x42, 
        0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x12, 0x10, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3A, 0x46, 0x42, 0x46, 0x3A, 0x02, 0x42, 0x3C, 0x00, 
        0x00, 0x00, 0x40, 0x40, 0x40, 0x5C, 0x62, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x08, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
        0x00, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x02, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x22, 
        0x24, 0x28, 0x34, 0x22, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 
        0x08, 0x08, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x49, 0x49, 0x49, 0x49, 
        0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5C, 0x62, 0x42, 0x42, 0x42, 0x42, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x5C, 0x62, 0x42, 0x62, 0x5C, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x3A, 0x46, 0x42, 0x46, 0x3A, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x5C, 0x62, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 
        0x42, 0x30, 0x0C, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x7C, 0x10, 0x10, 
        0x10, 0x12, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x46, 
        0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x41, 0x22, 0x22, 0x14, 0x08, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x41, 0x49, 0x49, 0x55, 0x22, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x46, 0x3A, 0x02, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x7E, 0x04, 0x08, 0x10, 0x20, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x08, 0x08, 0x08, 
        0x10, 0x08, 0x08, 0x08, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 
        0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x08, 0x08, 0x08, 0x04, 0x08, 0x08, 0x08, 
        0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x4C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x14, 0x22, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x1E, 0x21, 0x40, 0x40, 0x40, 0x40, 0x41, 0x22, 0x1E, 0x02, 0x1C, 0x00, 0x00, 0x00, 
        0x00, 0x24, 0x00, 0x42, 0x42, 0x42, 0x42, 0x46, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x10, 
        0x00, 0x3C, 0x42, 0x7E, 0x40, 0x40, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x08, 0x14, 0x22, 0x00, 0x3C, 
        0x02, 0x3E, 0x42, 0x42, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x3C, 0x02, 0x3E, 
        0x42, 0x42, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x04, 0x00, 0x3C, 0x02, 0x3E, 0x42, 0x42, 
        0x3D, 0x00, 0x00, 0x00, 0x00, 0x08, 0x14, 0x08, 0x00, 0x3C, 0x02, 0x3E, 0x42, 0x42, 0x3D, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x40, 0x40, 0x42, 0x3C, 0x04, 0x38, 0x00, 
        0x00, 0x18, 0x24, 0x42, 0x00, 0x3C, 0x42, 0x7E, 0x40, 0x40, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x24, 0x00, 0x3C, 0x42, 0x7E, 0x40, 0x40, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x04, 
        0x00, 0x3C, 0x42, 0x7E, 0x40, 0x40, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x18, 
        0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x08, 0x14, 0x22, 0x00, 0x18, 0x08, 0x08, 
        0x08, 0x08, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x08, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 
        0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x08, 0x14, 0x22, 0x41, 0x7F, 0x41, 0x41, 0x00, 
        0x00, 0x00, 0x08, 0x14, 0x08, 0x00, 0x08, 0x14, 0x22, 0x41, 0x7F, 0x41, 0x41, 0x00, 0x00, 0x00, 
        0x00, 0x0C, 0x10, 0x00, 0x7E, 0x40, 0x40, 0x7C, 0x40, 0x40, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x76, 0x09, 0x3F, 0x48, 0x48, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x18, 
        0x28, 0x48, 0x4E, 0x78, 0x48, 0x48, 0x4F, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x00, 0x3C, 
        0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x3C, 0x42, 0x42, 
        0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x04, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 
        0x3C, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x00, 0x42, 0x42, 0x42, 0x42, 0x46, 0x3A, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x30, 0x08, 0x00, 0x42, 0x42, 0x42, 0x42, 0x46, 0x3A, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x24, 0x00, 0x42, 0x42, 0x42, 0x46, 0x3A, 0x02, 0x42, 0x3C, 0x00, 0x00, 0x00, 
        0x22, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 
        0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x3C, 0x42, 
        0x40, 0x40, 0x42, 0x3C, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x20, 0x10, 0x3C, 0x10, 
        0x10, 0x32, 0x5C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x22, 0x14, 0x08, 0x3E, 0x08, 0x3E, 0x08, 
        0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x44, 0x44, 0x78, 0x44, 0x4E, 0x44, 0x44, 0x04, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x0C, 0x12, 0x10, 0x10, 0x3C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x60, 0x00, 
        0x00, 0x00, 0x0C, 0x10, 0x00, 0x3C, 0x02, 0x3E, 0x42, 0x42, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x0C, 0x10, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x10, 
        0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x10, 0x00, 0x42, 
        0x42, 0x42, 0x42, 0x46, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x4C, 0x00, 0x5C, 0x62, 0x42, 
        0x42, 0x42, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x26, 0x41, 0x61, 0x51, 0x49, 0x45, 0x43, 
        0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x04, 0x1C, 0x24, 0x1A, 0x00, 0x3E, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x22, 0x22, 0x1C, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x08, 0x00, 0x08, 0x08, 0x30, 0x40, 0x40, 0x41, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x7F, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x7F, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x62, 0x24, 0x28, 
        0x10, 0x2E, 0x02, 0x04, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x62, 0x24, 0x28, 0x10, 0x2A, 
        0x0A, 0x0F, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 
        0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x12, 0x24, 0x12, 0x09, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x12, 0x09, 0x12, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0xAA, 0x55, 
        0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0x77, 0xDD, 0x77, 0xDD, 
        0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xF8, 
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xF8, 0x08, 0xF8, 0x08, 0x08, 
        0x08, 0x08, 0x08, 0x08, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0xF4, 0x14, 0x14, 0x14, 0x14, 
        0x14, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x08, 0xF8, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x14, 0x14, 
        0x14, 0x14, 0x14, 0xF4, 0x04, 0xF4, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 
        0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 
        0x04, 0xF4, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0xF4, 0x04, 0xFC, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0xFC, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0xF8, 0x08, 0xF8, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 
        0x08, 0x08, 0x08, 0x08, 0x08, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0xFF, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
        0x08, 0x0F, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xFF, 0x08, 0x08, 
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0F, 0x08, 0x0F, 0x08, 0x08, 0x08, 0x08, 
        0x08, 0x08, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x17, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 
        0x14, 0x14, 0x14, 0x14, 0x14, 0x17, 0x10, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x1F, 0x10, 0x17, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 
        0x14, 0xF7, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
        0x00, 0xF7, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x17, 0x10, 0x17, 
        0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0xF7, 0x00, 0xF7, 0x14, 0x14, 0x14, 0x14, 
        0x14, 0x14, 0x08, 0x08, 0x08, 0x08, 0x08, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0xFF, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 
        0x14, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0F, 0x08, 0x0F, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x08, 0x0F, 0x08, 0x08, 
        0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x14, 0x14, 0x14, 0x14, 
        0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0xFF, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 
        0x08, 0x08, 0x08, 0x08, 0x08, 0xFF, 0x08, 0xFF, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
        0x08, 0x08, 0x08, 0x08, 0x08, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x0F, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 
        0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 
        0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x48, 0x48, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x1C, 0x22, 0x42, 0x5C, 0x44, 0x42, 0x42, 0x52, 0x4C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x42, 
        0x42, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 
        0x52, 0x12, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x21, 0x10, 0x08, 0x04, 0x08, 
        0x10, 0x21, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x44, 0x44, 0x44, 0x44, 
        0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x7C, 0x40, 0x40, 0x80, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x50, 0x10, 0x10, 0x14, 0x08, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x1C, 0x08, 0x3E, 0x49, 0x49, 0x49, 0x3E, 0x08, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x1C, 0x22, 0x41, 0x7F, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 
        0x41, 0x41, 0x41, 0x22, 0x14, 0x55, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x12, 0x08, 0x3C, 
        0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x49, 0x49, 
        0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1D, 0x22, 0x45, 0x49, 0x51, 0x22, 0x5C, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x20, 0x40, 0x7F, 0x40, 0x20, 0x1F, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x08, 0x08, 0x08, 0x7F, 0x08, 0x08, 0x08, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x08, 
        0x04, 0x02, 0x04, 0x08, 0x10, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x10, 0x20, 
        0x10, 0x08, 0x04, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x09, 0x0B, 0x08, 0x08, 0x08, 
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x68, 0x48, 
        0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x7F, 0x00, 0x08, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x4C, 0x00, 0x32, 0x4C, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x18, 0x24, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x18, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x08, 0x08, 0x08, 0x08, 
        0x48, 0x28, 0x18, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x12, 0x12, 0x12, 0x12, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x08, 0x10, 0x3C, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    /// @brief Default font instance(8x14)
    extern gfx::bitfont FONT_DEFAULT;
}