#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <array.h>
#include <gfx/color.h>
#include <gfx/font.h>
#include <gfx/primitives/rectangle.h>
#include <gfx/primitives/vec2d.h>

namespace std
{
    namespace gfx
    {
        /// @brief Structure for managing 32-bit raw image data
        class image
        {
            private:
                /// @brief Identification number used by asset manager
                uint32_t _id;
                /// @brief Width and height of image
                ivec2d_t _size;
                /// @brief Physical image data 
                std::array<uint32_t> _data;

            public:
                /// @brief Default image constructor - initializes all values to null
                image();
                /// @brief Create new image of specified size @param w Width of image @param h Height of image
                image(int w, int h);
                /// @brief Create new image of specified size @param size Image dimensions
                image(ivec2d_t size);
                /// @brief Create an image with exising data @param w Width of image @param h Height of image @param data Array containing raw image data
                image(int w, int h, std::array<uint32_t> data);
                /// @brief Create an image with exising data @param size Image dimensions @param data Array containing raw image data
                image(ivec2d_t size, std::array<uint32_t> data);
                /// @brief Load image from file @param fname Pointer to filename string
                image(char* fname);

                /// @brief Dispose all allocations used by this image
                void dispose();

                /// @brief Set identification number
                void set_id(uint32_t id);

                /// @brief Create an image with exising data @param w Width of image @param h Height of image @param data Array containing raw image data @return Image loaded successfully
                bool load(int w, int h, std::array<uint32_t> data);
                /// @brief Load image from file @param fname Pointer to filename string @return Image loaded successfully
                bool load(char* fname);

                /// @brief Resize image to specified dimensions @param w New width of image @param h New height of image
                void resize(int w, int h);
                /// @brief Resize image to specified dimensions @param size New image dimensions
                void resize(ivec2d_t size);

                /// @brief Fill entire image with specified color @param color 32-bit color value
                void clear(uint32_t color = 0xFF000000);
                /// @brief Fill entire image with specified color @param color 32-bit color value
                void clear(color32 color);

                /// @brief Draw pixel at specified position @param x X coordinate @param y Y coordinate @param color 32-bit color value
                void blit(int x, int y, uint32_t color = 0xFF000000);
                /// @brief Draw pixel at specified position @param x X coordinate @param y Y coordinate @param color 32-bit color value
                void blit(int x, int y, color32 color);
                /// @brief Draw pixel at specified position @param pos Coordinates @param color 32-bit color vvalue
                void blit(ivec2d_t pos, uint32_t color = 0xFF000000);
                /// @brief Draw pixel at specified position @param pos Coordinates @param color 32-bit color vvalue
                void blit(ivec2d_t pos, color32 color);

                /// @brief Draw filled rectangle of specified dimensions @param x X coordinate @param y Y coordinate @param w Width of rectangle @param h Height of rectangle @param color 32-bit color value
                void rect_filled(int x, int y, int w, int h, uint32_t color = 0xFF000000);
                /// @brief Draw filled rectangle of specified dimensions @param x X coordinate @param y Y coordinate @param w Width of rectangle @param h Height of rectangle @param color 32-bit color value
                void rect_filled(int x, int y, int w, int h, color32 color);
                /// @brief Draw filled rectangle of specified dimensions @param pos Coordinates @param size Size of rectangle @param color 32-bit color value
                void rect_filled(ivec2d_t pos, ivec2d_t size, uint32_t color = 0xFF000000);
                /// @brief Draw filled rectangle of specified dimensions @param pos Coordinates @param size Size of rectangle @param color 32-bit color value
                void rect_filled(ivec2d_t pos, ivec2d_t size, color32 color);
                /// @brief Draw filled rectangle @param bounds Rectangle @param color 32-bit color value
                void rect_filled(irect_t bounds, uint32_t color = 0xFF000000);
                /// @brief Draw filled rectangle @param bounds Rectangle @param color 32-bit color value
                void rect_filled(irect_t bounds, color32 color);

                /// @brief Draw rectangle outline of specified dimensions @param x X coordinate @param y Y coordinate @param w Width of rectangle @param h Height of rectangle @param t Thickness of outline @param color 32-bit color value
                void rect(int x, int y, int w, int h, int t, uint32_t color = 0xFF000000);
                /// @brief Draw rectangle outline of specified dimensions @param x X coordinate @param y Y coordinate @param w Width of rectangle @param h Height of rectangle @param t Thickness of outline @param color 32-bit color value
                void rect(int x, int y, int w, int h, int t, color32 color);
                /// @brief Draw rectangle outline of specified dimensions @param pos Coordinates @param size Size of rectangle @param t Thickness of outline @param color 32-bit color value
                void rect(ivec2d_t pos, ivec2d_t size, int t, uint32_t color = 0xFF000000);
                /// @brief Draw rectangle outline of specified dimensions @param pos Coordinates @param size Size of rectangle @param t Thickness of outline @param color 32-bit color value
                void rect(ivec2d_t pos, ivec2d_t size, int t, color32 color);
                /// @brief Draw rectangle outline of specified dimensions @param bounds Rectangle @param t Thickness of outline @param color 32-bit color value
                void rect(irect_t bounds, int t, uint32_t color = 0xFF000000);
                /// @brief Draw rectangle outline of specified dimensions @param bounds Rectangle @param t Thickness of outline @param color 32-bit color value
                void rect(irect_t bounds, int t, color32 color);

                /// @brief Draw character at specified position @param x X coordinate @param y Y coordinate @param c Character value @param font Bitfont @param fg 32-bit foreground color @param bg 32-bit background color
                void putc(int x, int y, char c, bitfont font, uint32_t fg, uint32_t bg = 0x00FFFFFF);
                /// @brief Draw character at specified position @param x X coordinate @param y Y coordinate @param c Character value @param font Bitfont @param fg 32-bit foreground color @param bg 32-bit background color
                void putc(int x, int y, char c, bitfont font, color32 fg, color32 bg);
                /// @brief Draw character at specified position @param pos Coordinates @param c Character value @param font Bitfont @param fg 32-bit foreground color @param bg 32-bit background color
                void putc(ivec2d_t pos, char c, bitfont font, uint32_t fg, uint32_t bg = 0x00FFFFFF);
                /// @brief Draw character at specified position @param pos Coordinates @param c Character value @param font Bitfont @param fg 32-bit foreground color @param bg 32-bit background color
                void putc(ivec2d_t pos, char c, bitfont font, color32 fg, color32 bg);

                /// @brief Draw string at specified position @param x X coordinate @param y Y coordinate @param str Pointer to string value @param font Bitfont @param fg 32-bit foreground color @param bg 32-bit background color
                void putstr(int x, int y, char* str, bitfont font, uint32_t fg, uint32_t bg = 0x00FFFFFF);
                /// @brief Draw string at specified position @param x X coordinate @param y Y coordinate @param str Pointer to string value @param font Bitfont @param fg 32-bit foreground color @param bg 32-bit background color
                void putstr(int x, int y, char* str, bitfont font, color32 fg, color32 bg);
                /// @brief Draw string at specified position @param pos Coordinates @param str Pointer to string value @param font Bitfont @param fg 32-bit foreground color @param bg 32-bit background color
                void putstr(ivec2d_t pos, char* str, bitfont font, uint32_t fg, uint32_t bg = 0x00FFFFFF);
                /// @brief Draw string at specified position @param pos Coordinates @param str Pointer to string value @param font Bitfont @param fg 32-bit foreground color @param bg 32-bit background color
                void putstr(ivec2d_t pos, char* str, bitfont font, color32 fg, color32 bg);

                /// @brief Copy raw data to image at specified position @param x X coordinate @param y Y coordinate @param sw Width of raw data @param sh Height of raw data @param data Pointer to raw data
                void copy(int x, int y, int sw, int sh, uint32_t* data);
                /// @brief Copy raw data to image at specified position @param x X coordinate @param y Y coordinate @param sw Width of raw data @param sh Height of raw data @param data Pointer to raw data
                void copy(int x, int y, int sw, int sh, color32* data);
                /// @brief Copy raw data to image at specified position @param pos Coordinates @param size Dimensions of raw data @param data Pointer to raw data
                void copy(ivec2d_t pos, ivec2d_t size, uint32_t* data);
                /// @brief Copy raw data to image at specified position @param pos Coordinates @param size Dimensions of raw data @param data Pointer to raw data
                void copy(ivec2d_t pos, ivec2d_t size, color32* data);
                /// @brief Copy raw data to image at specified position @param bounds Dimensions of raw data @param data Pointer to raw data
                void copy(irect_t bounds, uint32_t* data);
                /// @brief Copy raw data to image at specified position @param bounds Dimensions of raw data @param data Pointer to raw data
                void copy(irect_t bounds, color32* data);
                /// @brief Copy another image to this at specified position @param x X coordinate @param y Y coordinate @param img Pointer to source image
                void copy(int x, int y, image* img);
                /// @brief Copy another image to this at specified position @param pos Coordinates @param img Pointer to source image
                void copy(ivec2d_t pos, image* img);

                void copy(int x, int y, int sw, int sh, uint32_t trans, uint32_t* data);
                void copy(int x, int y, int sw, int sh, color32 trans, color32* data);
                void copy(ivec2d_t pos, ivec2d_t size, uint32_t trans, uint32_t* data);
                void copy(ivec2d_t pos, ivec2d_t size, color32 trans, color32* data);
                void copy(irect_t bounds, uint32_t trans, uint32_t* data);
                void copy(irect_t bounds, color32 trans, color32* data);
                void copy(int x, int y, uint32_t trans, image* img);
                void copy(int x, int y, color32 trans, image* img);
                void copy(ivec2d_t pos, uint32_t trans, image* img);
                void copy(ivec2d_t pos, color32 trans, image* img);

                void copy(int x, int y, irect_t src, uint32_t trans, image* img);
                void copy(int x, int y, irect_t src, color32 trans, image* img);

            public:
                /// @brief Get width and height of image @return 2d-vector containing width and height
                ivec2d_t size();
                /// @brief Get raw data of image @return Array containing raw image data
                array<uint32_t> data();
                /// @brief Get identification number
                uint32_t id();

        };
    }
}