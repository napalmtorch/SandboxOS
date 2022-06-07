#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <array.h>
#include <common.h>

namespace std
{
    /// @brief Template class for streaming a list of data
    template<typename T> class stream
    {
        private:
            /// @brief Pointer to item list
            T*     _data;
            /// @brief Amount of items
            int    _len;
            /// @brief Current stream position
            int    _pos;
            /// @brief Dummy value for returning from failed methods
            T      _dummy;

        public:
            /// @brief Default stream constructor - initializes with null values
            stream() { this->_data = NULL; this->_len = 0; this->_pos = 0; memset(&_dummy, 0, sizeof(T)); }

            /// @brief Create a new data stream with specified size @param len Amount of entries
            stream(size_t len)
            {
                if (len == 0) { this->_data = NULL; this->_len = 0; this->_pos = 0; return; }
                this->_data = (T*)tmalloc(sizeof(T) * len, ALLOCTYPE_ARRAY);
                this->_len  = (int)len;
                this->_pos  = 0;
                memset(&_dummy, 0, sizeof(T));
            }

            /// @brief Create a data stream with existing array of data @param data Data array @param bool Create a new copy of data
            stream(array<T> data, bool copy = false)
            {
                this->_len = data.length();
                this->_pos = 0;
                if (copy) { this->data = (T*)tmalloc(sizeof(T) * data.length(), ALLOCTYPE_ARRAY); memcpy(this->_data, data.ptr(), sizeof(T) * data.length()); }
                else { this->data = data.ptr(); }
                memset(&_dummy, 0, sizeof(T));
            }

            /// @brief Create a data stream with existing data @param data Pointer to list @param bool Create a new copy of data
            stream(T* data, size_t len, bool copy = false)
            {
                this->_len  = (int)len;
                this->_pos  = 0;
                if (copy) { this->_data = (T*)tmalloc(sizeof(T) * len, ALLOCTYPE_ARRAY); memcpy(this->_data, data, sizeof(T) * len); }
                else { this->_data = data; }
                memset(&_dummy, 0, sizeof(T));
            }

            /// @brief Dispose stream data and reset properties
            void dispose()
            {
                if (this->_data != NULL) { free(this->_data); this->_data = NULL; }
                this->_len = 0;
                this->_pos = 0;
            }

            /// @brief Clear all entries from stream
            void clear() { memset(this->_data, 0, sizeof(T) * this->_len); }

            /// @brief Goto specified position in stream @param pos Index in list
            void seek(int pos)
            {
                if (this->_data == NULL) { perror("Null array in stream at 0x%8x", this); return; }
                if (pos < 0 || pos >= this->_len) { perror("Seek out of bounds for stream at 0x%8x - %u", this, pos); return; }
                this->_pos = pos;
            }

            /// @brief Peek value at current position in stream @return Item at current position
            T peek()
            {
                if (this->_data == NULL) { perror("Null array in stream at 0x%8x", this); return _dummy; }
                if (this->_pos - 1 < 0 || this->_pos - 1 >= this->_len) { perror("Peek out of bounds for stream at 0x%8x - %u", this, this->_pos); return _dummy; }
                return this->_data[this->_pos - 1];
            }

            /// @brief Fetch next value in stream and increment position @return Next item in list
            T next()
            {
                if (this->_data == NULL) { perror("Null array in stream at 0x%8x", this); return _dummy; }
                if (this->_pos < 0 || this->_pos >= this->_len) { return _dummy; }
                T val = this->_data[this->_pos++];
                return val;
            }

            /// @brief Check if stream contains any more valid items @return Stream has finished
            bool done()
            {
                if (this->_data == NULL) { return true; }
                if (this->_pos < 0 || this->_pos >= this->_len) { return true; }
                return false;
            }

            /// @brief Access value in stream by index @param index Index of item
            T& operator[](int index) { return at(index); }

            /// @brief Access value in stream by index @param index Index of item @return Reference to item
            T& at(int index)
            {
                if (this->_data == NULL) { perror("Null data for stream at 0x%8x", this); return _dummy; }
                if (index < 0 || index >= (int)this->_len) { perror("Index out of bounds for stream at 0x%8x", this); return _dummy; }
                return this->_data[index];
            }

            /// @brief Get pointer to data list @return Pointer to data
            T* ptr() { return this->_data; }

            /// @brief Get amount of entries
            size_t length() { return (size_t)this->_len; }

            /// @brief Get current position in stream
            int tell() { return this->_pos; }
    };
}