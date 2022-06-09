#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

namespace std
{
    struct ivec2d_t
    {
        public:
            int x, y;

        public:
            ivec2d_t() { this->x = 0; this->y = 0; }
            ivec2d_t(int x, int y) { this->x = x; this->y = y; }
            ivec2d_t(ivec2d_t& vec) { this->x = vec.x; this->y = vec.y; }

            bool equals(int x, int y) { return this->x == x && this->y == y; }
            bool equals(ivec2d_t& vec) { return this->x == vec.x && this->y == vec.y; }
            bool equals(int value) { return this->x == value && this->y == value; }

            ivec2d_t& add(int x, int y) { this->x += x; this->y += y; return *this; }
            ivec2d_t& add(ivec2d_t& vec) { this->x += vec.x; this->y += vec.y; return *this; }
            ivec2d_t& add(int value) { this->x += value; this->y += value; return *this; }

            ivec2d_t& sub(int x, int y) { this->x -= x; this->y -= y; return *this; }
            ivec2d_t& sub(ivec2d_t& vec) { this->x -= vec.x; this->y -= vec.y; return *this; }
            ivec2d_t& sub(int value) { this->x -= value; this->y -= value; return *this; }

            ivec2d_t& mul(int x, int y) { this->x *= x; this->y *= y; return *this; }
            ivec2d_t& mul(ivec2d_t& vec) { this->x *= vec.x; this->y *= vec.y; return *this; }
            ivec2d_t& mul(int value) { this->x *= value; this->y *= value; return *this; }

            ivec2d_t& div(int x, int y) { this->x /= x; this->y /= y; return *this; }
            ivec2d_t& div(ivec2d_t& vec) { this->x /= vec.x; this->y /= vec.y; return *this; }
            ivec2d_t& div(int value) { this->x /= value; this->y /= value; return *this; }
    };

    struct fvec2d_t
    {
        public:
            float x, y;

        public:
            fvec2d_t() { this->x = 0; this->y = 0; }
            fvec2d_t(float x, float y) { this->x = x; this->y = y; }
            fvec2d_t(fvec2d_t& vec) { this->x = vec.x; this->y = vec.y; }

            bool equals(float x, float y) { return this->x == x && this->y == y; }
            bool equals(fvec2d_t& vec) { return this->x == vec.x && this->y == vec.y; }
            bool equals(float value) { return this->x == value && this->y == value; }

            fvec2d_t& add(float x, float y) { this->x += x; this->y += y; return *this; }
            fvec2d_t& add(fvec2d_t& vec) { this->x += vec.x; this->y += vec.y; return *this; }
            fvec2d_t& add(float value) { this->x += value; this->y += value; return *this; }

            fvec2d_t& sub(float x, float y) { this->x -= x; this->y -= y; return *this; }
            fvec2d_t& sub(fvec2d_t& vec) { this->x -= vec.x; this->y -= vec.y; return *this; }
            fvec2d_t& sub(float value) { this->x -= value; this->y -= value; return *this; }

            fvec2d_t& mul(float x, float y) { this->x *= x; this->y *= y; return *this; }
            fvec2d_t& mul(fvec2d_t& vec) { this->x *= vec.x; this->y *= vec.y; return *this; }
            fvec2d_t& mul(float value) { this->x *= value; this->y *= value; return *this; }

            fvec2d_t& div(float x, float y) { this->x /= x; this->y /= y; return *this; }
            fvec2d_t& div(fvec2d_t& vec) { this->x /= vec.x; this->y /= vec.y; return *this; }
            fvec2d_t& div(float value) { this->x /= value; this->y /= value; return *this; }
    };
}