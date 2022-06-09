#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <gfx/primitives/vec2d.h>

namespace std
{
    struct irect_t
    {
        public:
            int x, y, w, h;

        public:
            irect_t() { this->x = 0; this->y = 0; this->w = 0; this->h = 0; }
            irect_t(int x, int y, int w, int h) { this->x = x; this->y = y; this->w = w; this->h = h; }
            irect_t(ivec2d_t pos, ivec2d_t size) { this->x = pos.x; this->y = pos.y; this->w = size.x; this->h = size.y; }
            irect_t(irect_t& rect) { this->x = rect.x; this->y = rect.y; this->w = rect.w; this->h = rect.h; }

        public:
            bool equals(int x, int y, int w, int h) { return (this->x == x && this->y == y && this->w == w && this->h == h); }
            bool equals(ivec2d_t pos, ivec2d_t size) { return equals(pos.x, pos.y, size.x, size.y); }
            bool equals(irect_t& rect) { return equals(rect.x, rect.y, rect.w, rect.h); }
            
            bool contains(int x, int y) { return (x >= this->x && y >= this->y && x < this->x + this->w && y < this->y + this->h); }
            bool contains(ivec2d_t pos) { return contains(pos.x, pos.y); }
            bool contains(irect_t& rect) { return contains(rect.x, rect.y); }
    };

    struct frect_t
    {
        public:
            float x, y, w, h;

        public:
            frect_t() { this->x = 0; this->y = 0; this->w = 0; this->h = 0; }
            frect_t(float x, float y, float w, float h) { this->x = x; this->y = y; this->w = w; this->h = h; }
            frect_t(fvec2d_t pos, fvec2d_t size) { this->x = pos.x; this->y = pos.y; this->w = size.x; this->h = size.y; }
            frect_t(frect_t& rect) { this->x = rect.x; this->y = rect.y; this->w = rect.w; this->h = rect.h; }

        public:
            bool equals(float x, float y, float w, float h) { return (this->x == x && this->y == y && this->w == w && this->h == h); }
            bool equals(fvec2d_t pos, fvec2d_t size) { return equals(pos.x, pos.y, size.x, size.y); }
            bool equals(frect_t& rect) { return equals(rect.x, rect.y, rect.w, rect.h); }
            
            bool contains(float x, float y) { return (x >= this->x && y >= this->y && x < this->x + this->w && y < this->y + this->h); }
            bool contains(fvec2d_t pos) { return contains(pos.x, pos.y); }
            bool contains(frect_t& rect) { return contains(rect.x, rect.y); }
    };
}