#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <gfx/primitives/vec2d.h>

namespace std
{
    /// @brief Structure for managing integer rectangles
    struct irect_t
    {
        public:
            /// @brief X coordinate
            int x;
            /// @brief Y coordinate
            int y;
            /// @brief Rectangle width
            int w;
            /// @brief Rectangle height
            int h;

        public:
            /// @brief Default rectangle constructor - initializes all values to zero
            irect_t() { this->x = 0; this->y = 0; this->w = 0; this->h = 0; }
            /// @brief Create rectangle with specified dimensions @param x X coordinate @param y Y coordinate @param w Rectangle width @param h Rectangle height
            irect_t(int x, int y, int w, int h) { this->x = x; this->y = y; this->w = w; this->h = h; }
            /// @brief Create rectangle with specified position and size @param pos Rectangle coordinates @param size Rectangle size
            irect_t(ivec2d_t pos, ivec2d_t size) { this->x = pos.x; this->y = pos.y; this->w = size.x; this->h = size.y; }
            /// @brief Create copy of existing rectangle @param rect Reference to existing rectangle
            irect_t(const irect_t& rect) { this->x = rect.x; this->y = rect.y; this->w = rect.w; this->h = rect.h; }

        public:
            /// @brief Check if rectangle dimensions are equivalent to specified @param x X coordinate @param y Y coordinate @param w Rectangle width @param h Rectangle height @return Rectangles are identical
            bool equals(int x, int y, int w, int h) { return (this->x == x && this->y == y && this->w == w && this->h == h); }
            /// @brief Check if rectangle dimensions are equivalent to specified position and size @param pos Rectangle coordinates @param size Rectangle size @return Rectangles are identical
            bool equals(ivec2d_t pos, ivec2d_t size) { return equals(pos.x, pos.y, size.x, size.y); }
            /// @brief Check if rectangle dimensions are equivalent to existing rectangle @param rect Reference to existing rectangle @return Rectangles are identical
            bool equals(const irect_t& rect) { return equals(rect.x, rect.y, rect.w, rect.h); }
            
            /// @brief Check if coordinates are within rectangle bounds @param x X coordinate @param y Y coordinate @return Coordinates are within rectangle
            bool contains(int x, int y) { return (x >= this->x && y >= this->y && x < this->x + this->w && y < this->y + this->h); }
            /// @brief Check if coordinates are within rectangle bounds @param pos Rectangle coordinates @return Coordinates are within rectangle
            bool contains(ivec2d_t pos) { return contains(pos.x, pos.y); }
            /// @brief Check if position of existing rectangle is within rectangle bounds @param rect Reference to existing rectangle @return Coordinates are within rectangle
            bool contains(const irect_t& rect) { return contains(rect.x, rect.y); }
    };

    /// @brief Structure for managing floating-point rectangles
    struct frect_t
    {
        public:
            /// @brief X coordinate
            float x;
            /// @brief Y coordinate
            float y;
            /// @brief Rectangle width
            float w;
            /// @brief Rectangle height
            float h;

        public:
            /// @brief Default rectangle constructor - initializes all values to zero
            frect_t() { this->x = 0; this->y = 0; this->w = 0; this->h = 0; }
            /// @brief Create rectangle with specified dimensions @param x X coordinate @param y Y coordinate @param w Rectangle width @param h Rectangle height
            frect_t(float x, float y, float w, float h) { this->x = x; this->y = y; this->w = w; this->h = h; }
            /// @brief Create rectangle with specified position and size @param pos Rectangle coordinates @param size Rectangle size
            frect_t(fvec2d_t pos, fvec2d_t size) { this->x = pos.x; this->y = pos.y; this->w = size.x; this->h = size.y; }
            /// @brief Create copy of existing rectangle @param rect Reference to existing rectangle
            frect_t(const frect_t& rect) { this->x = rect.x; this->y = rect.y; this->w = rect.w; this->h = rect.h; }

        public:
            /// @brief Check if rectangle dimensions are equivalent to specified @param x X coordinate @param y Y coordinate @param w Rectangle width @param h Rectangle height @return Rectangles are identical
            bool equals(float x, float y, float w, float h) { return (this->x == x && this->y == y && this->w == w && this->h == h); }
            /// @brief Check if rectangle dimensions are equivalent to specified position and size @param pos Rectangle coordinates @param size Rectangle size @return Rectangles are identical
            bool equals(fvec2d_t pos, fvec2d_t size) { return equals(pos.x, pos.y, size.x, size.y); }
            /// @brief Check if rectangle dimensions are equivalent to existing rectangle @param rect Reference to existing rectangle @return Rectangles are identical
            bool equals(const frect_t& rect) { return equals(rect.x, rect.y, rect.w, rect.h); }
            
            /// @brief Check if coordinates are within rectangle bounds @param x X coordinate @param y Y coordinate @return Coordinates are within rectangle
            bool contains(float x, float y) { return (x >= this->x && y >= this->y && x < this->x + this->w && y < this->y + this->h); }
            /// @brief Check if coordinates are within rectangle bounds @param pos Rectangle coordinates @return Coordinates are within rectangle
            bool contains(fvec2d_t pos) { return contains(pos.x, pos.y); }
            /// @brief Check if position of existing rectangle is within rectangle bounds @param rect Reference to existing rectangle @return Coordinates are within rectangle
            bool contains(const frect_t& rect) { return contains(rect.x, rect.y); }
    };
}