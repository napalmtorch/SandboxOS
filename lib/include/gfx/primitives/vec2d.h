#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

namespace std
{
    /// @brief Structure for managing 2-dimensional integer vectors
    struct ivec2d_t
    {
        public:
            /// @brief X coordinate
            int x;
            /// @brief Y coordinate
            int y;

        public:
            /// @brief Default vec2d constructor - initializes all values to zero
            ivec2d_t() { this->x = 0; this->y = 0; }
            /// @brief Create vec2d with specified coordinates @param x X coordinate @param y Y coordinate
            ivec2d_t(int x, int y) { this->x = x; this->y = y; }
            /// @brief Create copy of existing vec2d @param vec Reference to existing vec2d
            ivec2d_t(ivec2d_t& vec) { this->x = vec.x; this->y = vec.y; }

            /// @brief Check if vec2d coordinates are equivalent to specified @param x X coordinate @param y Y coordinate @return Coordinates are identical
            bool equals(int x, int y) { return this->x == x && this->y == y; }
            /// @brief Check if vec2d coordinates are equivalent to existing vec2d @param vec Reference to existing vec2d @return Coordinates are identical
            bool equals(ivec2d_t& vec) { return this->x == vec.x && this->y == vec.y; }
            /// @brief Check if vec2d coordinates are equal to value @param value Integer value @return Coordinates are identical to value
            bool equals(int value) { return this->x == value && this->y == value; }

            /// @brief Add values to coordinates @param x X coordinate @param y Y coordinate @return Reference to self after operation
            ivec2d_t& add(int x, int y) { this->x += x; this->y += y; return *this; }
            /// @brief Add values to coordinates @param vec Reference to existing vec2d @return Reference to self after operation
            ivec2d_t& add(ivec2d_t& vec) { this->x += vec.x; this->y += vec.y; return *this; }
            /// @brief Add value to coordinates @param value Right-hand side value @return Reference to self after operation
            ivec2d_t& add(int value) { this->x += value; this->y += value; return *this; }

            /// @brief Subtract values to coordinates @param x X coordinate @param y Y coordinate @return Reference to self after operation
            ivec2d_t& sub(int x, int y) { this->x -= x; this->y -= y; return *this; }
            /// @brief Subtract values to coordinates @param vec Reference to existing vec2d @return Reference to self after operation
            ivec2d_t& sub(ivec2d_t& vec) { this->x -= vec.x; this->y -= vec.y; return *this; }
            /// @brief Subtract value to coordinates @param value Right-hand side value @return Reference to self after operation
            ivec2d_t& sub(int value) { this->x -= value; this->y -= value; return *this; }

            /// @brief Multiply values to coordinates @param x X coordinate @param y Y coordinate @return Reference to self after operation
            ivec2d_t& mul(int x, int y) { this->x *= x; this->y *= y; return *this; }
            /// @brief Multiply values to coordinates @param vec Reference to existing vec2d @return Reference to self after operation
            ivec2d_t& mul(ivec2d_t& vec) { this->x *= vec.x; this->y *= vec.y; return *this; }
            /// @brief Multiply value to coordinates @param value Right-hand side value @return Reference to self after operation
            ivec2d_t& mul(int value) { this->x *= value; this->y *= value; return *this; }
    
            /// @brief Divide values to coordinates @param x X coordinate @param y Y coordinate @return Reference to self after operation        
            ivec2d_t& div(int x, int y) { this->x /= x; this->y /= y; return *this; }
            /// @brief Divide values to coordinates @param vec Reference to existing vec2d @return Reference to self after operation
            ivec2d_t& div(ivec2d_t& vec) { this->x /= vec.x; this->y /= vec.y; return *this; }
            /// @brief Divide value to coordinates @param value Right-hand side value @return Reference to self after operation
            ivec2d_t& div(int value) { this->x /= value; this->y /= value; return *this; }
    };

    /// @brief Structure for managing 2-dimensional floating-point vectors
    struct fvec2d_t
    {
        public:
            /// @brief X coordinate
            float x;
            /// @brief Y coordinate
            float y;

        public:
            /// @brief Default vec2d constructor - initializes all values to zero
            fvec2d_t() { this->x = 0; this->y = 0; }
            /// @brief Create vec2d with specified coordinates @param x X coordinate @param y Y coordinate
            fvec2d_t(float x, float y) { this->x = x; this->y = y; }
            /// @brief Create copy of existing vec2d @param vec Reference to existing vec2d
            fvec2d_t(fvec2d_t& vec) { this->x = vec.x; this->y = vec.y; }

            /// @brief Check if vec2d coordinates are equivalent to specified @param x X coordinate @param y Y coordinate @return Coordinates are identical
            bool equals(float x, float y) { return this->x == x && this->y == y; }
            /// @brief Check if vec2d coordinates are equivalent to existing vec2d @param vec Reference to existing vec2d @return Coordinates are identical
            bool equals(fvec2d_t& vec) { return this->x == vec.x && this->y == vec.y; }
            /// @brief Check if vec2d coordinates are equal to value @param value Integer value @return Coordinates are identical to value
            bool equals(float value) { return this->x == value && this->y == value; }

            /// @brief Add values to coordinates @param x X coordinate @param y Y coordinate @return Reference to self after operation
            fvec2d_t& add(float x, float y) { this->x += x; this->y += y; return *this; }
            /// @brief Add values to coordinates @param vec Reference to existing vec2d @return Reference to self after operation
            fvec2d_t& add(fvec2d_t& vec) { this->x += vec.x; this->y += vec.y; return *this; }
            /// @brief Add value to coordinates @param value Right-hand side value @return Reference to self after operation
            fvec2d_t& add(float value) { this->x += value; this->y += value; return *this; }

            /// @brief Subtract values to coordinates @param x X coordinate @param y Y coordinate @return Reference to self after operation
            fvec2d_t& sub(float x, float y) { this->x -= x; this->y -= y; return *this; }
            /// @brief Subtract values to coordinates @param vec Reference to existing vec2d @return Reference to self after operation
            fvec2d_t& sub(fvec2d_t& vec) { this->x -= vec.x; this->y -= vec.y; return *this; }
            /// @brief Subtract value to coordinates @param value Right-hand side value @return Reference to self after operation
            fvec2d_t& sub(float value) { this->x -= value; this->y -= value; return *this; }

            /// @brief Multiply values to coordinates @param x X coordinate @param y Y coordinate @return Reference to self after operation
            fvec2d_t& mul(float x, float y) { this->x *= x; this->y *= y; return *this; }
            /// @brief Multiply values to coordinates @param vec Reference to existing vec2d @return Reference to self after operation
            fvec2d_t& mul(fvec2d_t& vec) { this->x *= vec.x; this->y *= vec.y; return *this; }
            /// @brief Multiply value to coordinates @param value Right-hand side value @return Reference to self after operation
            fvec2d_t& mul(float value) { this->x *= value; this->y *= value; return *this; }
    
            /// @brief Divide values to coordinates @param x X coordinate @param y Y coordinate @return Reference to self after operation        
            fvec2d_t& div(float x, float y) { this->x /= x; this->y /= y; return *this; }
            /// @brief Divide values to coordinates @param vec Reference to existing vec2d @return Reference to self after operation
            fvec2d_t& div(fvec2d_t& vec) { this->x /= vec.x; this->y /= vec.y; return *this; }
            /// @brief Divide value to coordinates @param value Right-hand side value @return Reference to self after operation
            fvec2d_t& div(float value) { this->x /= value; this->y /= value; return *this; }
    };
}