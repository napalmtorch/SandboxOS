#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <array.h>
#include <common.h>

namespace std
{
    template<typename T> class stream
    {
        private:
            T*     _data;
            int    _len;
            int    _pos;
            T      _dummy;

        public:
            stream() { this->_data = NULL; this->_len = 0; this->_pos = 0; memset(&_dummy, 0, sizeof(T)); }

            stream(size_t len)
            {
                if (len == 0) { this->_data = NULL; this->_len = 0; this->_pos = 0; return; }
                this->_data = (T*)tmalloc(sizeof(T) * len, ALLOCTYPE_ARRAY);
                this->_len  = (int)len;
                this->_pos  = 0;
                memset(&_dummy, 0, sizeof(T));
            }

            stream(array<T> data, bool copy = false)
            {
                this->_len = data.length();
                this->_pos = 0;
                if (copy) { this->data = (T*)tmalloc(sizeof(T) * data.length(), ALLOCTYPE_ARRAY); memcpy(this->_data, data.ptr(), sizeof(T) * data.length()); }
                else { this->data = data.ptr(); }
                memset(&_dummy, 0, sizeof(T));
            }

            stream(T* data, size_t len, bool copy = false)
            {
                this->_len  = (int)len;
                this->_pos  = 0;
                if (copy) { this->_data = (T*)tmalloc(sizeof(T) * len, ALLOCTYPE_ARRAY); memcpy(this->_data, data, sizeof(T) * len); }
                else { this->_data = data; }
                memset(&_dummy, 0, sizeof(T));
            }

            void dispose()
            {
                if (this->_data != NULL) { free(this->_data); this->_data = NULL; }
                this->_len = 0;
                this->_pos = 0;
            }

            void clear() { memset(this->_data, 0, sizeof(T) * this->_len); }

            void seek(int pos)
            {
                if (this->_data == NULL) { perror("Null array in stream at 0x%8x", this); return; }
                if (pos < 0 || pos >= this->_len) { perror("Seek out of bounds for stream at 0x%8x - %u", this, pos); return; }
                this->_pos = pos;
            }

            T peek()
            {
                if (this->_data == NULL) { perror("Null array in stream at 0x%8x", this); return _dummy; }
                if (this->_pos - 1 < 0 || this->_pos - 1 >= this->_len) { perror("Peek out of bounds for stream at 0x%8x - %u", this, this->_pos); return _dummy; }
                return this->_data[this->_pos - 1];
            }

            T next()
            {
                if (this->_data == NULL) { perror("Null array in stream at 0x%8x", this); return _dummy; }
                if (this->_pos < 0 || this->_pos >= this->_len) { return _dummy; }
                T val = this->_data[this->_pos++];
                return val;
            }

            bool done()
            {
                if (this->_data == NULL) { return true; }
                if (this->_pos < 0 || this->_pos >= this->_len) { return true; }
                return false;
            }

            T& operator[](int index) { return at(index); }

            T& at(int index)
            {
                if (this->_data == NULL) { perror("Null data for stream at 0x%8x", this); return _dummy; }
                if (index < 0 || index >= (int)this->_len) { perror("Index out of bounds for stream at 0x%8x", this); return _dummy; }
                return this->_data[index];
            }

            T* ptr() { return this->_data; }

            size_t length() { return (size_t)this->_len; }

            int tell() { return this->_pos; }
    };
}