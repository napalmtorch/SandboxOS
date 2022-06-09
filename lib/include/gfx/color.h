#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

namespace std
{
    /// @brief Structure for managing 32-bit ARGB color values
    typedef struct
    {
        /// @brief 8-bit alpha value
        uint8_t a;
        /// @brief 8-bit red value
        uint8_t r;
        /// @brief 8-bit green value
        uint8_t g;
        /// @Brief 8-bit blue value
        uint8_t b;
    } PACKED argb_t;

    /// @brief 4-bit color codes
    enum class color4 : uint8_t
    {
        black,
        dark_blue,
        dark_green,
        dark_cyan,
        dark_red,
        dark_magenta,
        dark_yellow,
        gray,
        dark_gray,
        blue,
        green,
        cyan,
        red,
        magenta,
        yellow,
        white,
    };

    /// @brief 32-bit color codes
    enum class color32 : uint32_t
    {
        transparent = 0x00FFFFFF,
        alice_blue = 0xFFF0F8FF,
        antique_white = 0xFFFAEBD7,
        aqua = 0xFF00FFFF,
        aquamarine = 0xFF7FFFD4,
        azure = 0xFFF0FFFF,
        beige = 0xFFF5F5DC,
        bisque = 0xFFFFE4C4,
        black = 0xFF000000,
        blanched_almond = 0xFFFFEBCD,
        blue = 0xFF0000FF,
        blue_violet = 0xFF8A2BE2,
        brown = 0xFFA52A2A,
        burly_wood = 0xFFDEB887,
        cadet_blue = 0xFF5F9EA0,
        chartreuse = 0xFF7FFF00,
        chocolate = 0xFFD2691E,
        coral = 0xFFFF7F50,
        cornflower_blue = 0xFF6495ED,
        cornsilk = 0xFFFFF8DC,
        crimson = 0xFFDC143C,
        cyan = 0xFF00FFFF,
        dark_blue = 0xFF00008B,
        dark_cyan = 0xFF008B8B,
        dark_goldenrod = 0xFFB8860B,
        dark_gray = 0xFFA9A9A9,
        dark_green = 0xFF006400,
        dark_khaki = 0xFFBDB76B,
        dark_magenta = 0xFF8B008B,
        dark_olive_green = 0xFF556B2F,
        dark_orange = 0xFFFF8C00,
        dark_orchid = 0xFF9932CC,
        dark_red = 0xFF8B0000,
        dark_salmon = 0xFFE9967A,
        dark_sea_green = 0xFF8FBC8B,
        dark_slate_blue = 0xFF483D8B,
        dark_slate_gray = 0xFF2F4F4F,
        dark_turquoise = 0xFF00CED1,
        dark_violet = 0xFF9400D3,
        deep_pink = 0xFFFF1493,
        deep_sky_blue = 0xFF00BFFF,
        dim_gray = 0xFF696969,
        dodger_blue = 0xFF1E90FF,
        firebrick = 0xFFB22222,
        floral_white = 0xFFFFFAF0,
        forest_green = 0xFF228B22,
        fuchsia = 0xFFFF00FF,
        gainsboro = 0xFFDCDCDC,
        ghost_white = 0xFFF8F8FF,
        gold = 0xFFFFD700,
        goldenrod = 0xFFDAA520,
        gray = 0xFF808080,
        green = 0xFF008000,
        green_yellow = 0xFFADFF2F,
        honeydew = 0xFFF0FFF0,
        hot_pink = 0xFFFF69B4,
        indian_red = 0xFFCD5C5C,
        indigo = 0xFF4B0082,
        ivory = 0xFFFFFFF0,
        khaki = 0xFFF0E68C,
        lavender = 0xFFE6E6FA,
        lavender_blush = 0xFFFFF0F5,
        lawn_green = 0xFF7CFC00,
        lemon_chiffon = 0xFFFFFACD,
        light_blue = 0xFFADD8E6,
        light_coral = 0xFFF08080,
        light_cyan = 0xFFE0FFFF,
        light_goldenrod_yellow = 0xFFFAFAD2,
        light_gray = 0xFFD3D3D3,
        light_green = 0xFF90EE90,
        light_pink = 0xFFFFB6C1,
        light_salmon = 0xFFFFA07A,
        light_sea_green = 0xFF20B2AA,
        light_sky_blue = 0xFF87CEFA,
        light_slate_gray = 0xFF778899,
        light_steel_blue = 0xFFB0C4DE,
        light_yellow = 0xFFFFFFE0,
        lime = 0xFF00FF00,
        lime_green = 0xFF32CD32,
        linen = 0xFFFAF0E6,
        magenta = 0xFFFF00FF,
        maroon = 0xFF800000,
        medium_aquamarine = 0xFF66CDAA,
        medium_blue = 0xFF0000CD,
        medium_orchid = 0xFFBA55D3,
        medium_purple = 0xFF9370DB,
        medium_sea_green = 0xFF3CB371,
        medium_slate_blue = 0xFF7B68EE,
        medium_spring_green = 0xFF00FA9A,
        medium_turquoise = 0xFF48D1CC,
        medium_violet_red = 0xFFC71585,
        midnight_blue = 0xFF191970,
        mint_cream = 0xFFF5FFFA,
        misty_rose = 0xFFFFE4E1,
        moccasin = 0xFFFFE4B5,
        navajo_white = 0xFFFFDEAD,
        navy = 0xFF000080,
        old_lace = 0xFFFDF5E6,
        olive = 0xFF808000,
        olive_drab = 0xFF6B8E23,
        orange = 0xFFFFA500,
        orange_red = 0xFFFF4500,
        orchid = 0xFFDA70D6,
        pale_goldenrod = 0xFFEEE8AA,
        pale_green = 0xFF98FB98,
        pale_turquoise = 0xFFAFEEEE,
        pale_violet_red = 0xFFDB7093,
        papaya_whip = 0xFFFFEFD5,
        peach_puff = 0xFFFFDAB9,
        peru = 0xFFCD853F,
        pink = 0xFFFFC0CB,
        plum = 0xFFDDA0DD,
        powder_blue = 0xFFB0E0E6,
        purple = 0xFF800080,
        red = 0xFFFF0000,
        rosy_brown = 0xFFBC8F8F,
        royal_blue = 0xFF4169E1,
        saddle_brown = 0xFF8B4513,
        salmon = 0xFFFA8072,
        sandy_brown = 0xFFF4A460,
        sea_green = 0xFF2E8B57,
        sea_shell = 0xFFFFF5EE,
        sienna = 0xFFA0522D,
        silver = 0xFFC0C0C0,
        sky_blue = 0xFF87CEEB,
        slate_blue = 0xFF6A5ACD,
        slate_gray = 0xFF708090,
        snow = 0xFFFFFAFA,
        spring_green = 0xFF00FF7F,
        steel_blue = 0xFF4682B4,
        tan = 0xFFD2B48C,
        teal = 0xFF008080,
        thistle = 0xFFD8BFD8,
        tomato = 0xFFFF6347,
        turquoise = 0xFF40E0D0,
        violet = 0xFFEE82EE,
        wheat = 0xFFF5DEB3,
        white = 0xFFFFFFFF,
        white_smoke = 0xFFF5F5F5,
        yellow = 0xFFFFFF00,
        yellow_green = 0xFF9ACD32,
    };

    namespace gfx
    {
        /// @brief Palette used for 4-bit to 32-bit color conversions
        static const uint32_t PALETTE_4BIT[16]
        {
            0xFF000000, 0xFF000080, 0xFF008000, 0xFF008080, 0xFF800000, 0xFF800080, 0xFF808000, 0xFFC0C0C0, 
            0xFF808080, 0xFF8080FF, 0xFF00FF00, 0xFF00FFFF, 0xFFFF0000, 0xFFFF00FF, 0xFFFFFF00, 0xFFFFFFFF,
        };

        /// @brief Convert 32-bit color to ARGB color structure @param color 32-bit color value @return ARGB color
        static inline argb_t color32_to_argb(uint32_t color)
        {
            return { (color & 0xFF000000) >> 24, (color & 0xFF0000) >> 16, (color & 0xFF00) >> 8, color & 0xFF };
        }

        /// @brief Convert 32-bit color to ARGB color structure @param color 32-bit color value @return ARGB color
        static inline argb_t color32_to_argb(color32 color) { return color32_to_argb((uint32_t)color); }

        /// @brief Convert ARGB color structure to 32-bit packed value @param argb ARGB color value @return 32-bit packed color value
        static inline color32 argb_to_color32(argb_t argb)
        {
            return (color32)((argb.a << 24) | (argb.r << 16) | (argb.g << 8) | argb.b);
        }

        /// @brief Convert 4-bit color to 32-bit color @param color 4-bit color value @return 32-bit color value
        static inline color32 color4_to_color32(color4 color)
        {
            if ((uint8_t)color >= 16) { return color32::black; }
            return (color32)PALETTE_4BIT[(int)color];
        }

        /// @brief Convert 32-bit color to 4-bit color @param color 32-bit color value @return 4-bit color
        static inline color4 color32_to_color4(uint32_t color)
        {
            argb_t argb = color32_to_argb(color);
            uint32_t f = 128;
            argb.r /= f;
            argb.g /= f;
            argb.b /= f;
            argb.r *= f;
            argb.g *= f;
            argb.b *= f;
            color32 conv = argb_to_color32(argb);
            for (size_t i = 0; i < 16; i++) { if (PALETTE_4BIT[i] == (uint32_t)conv) { return (color4)i; } }
            return color4::black;
        }

        /// @brief Convert 32-bit color to 4-bit color @param color 32-bit color value @return 4-bit color
        static inline color4 color32_to_color4(color32 color) { return color32_to_color4((uint32_t)color); }
    }
}