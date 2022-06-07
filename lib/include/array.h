#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <common.h>

namespace std
{
    template<typename T> class array
    {
        private:
            T*     _data;
            size_t _len;  
            T      _dummy;

        public:
            array() { this->_data = NULL; this->_len = 0; }
            
            array(size_t len) 
            { 
                if (len == 0) { this->_data = NULL; this->_len = 0; return; }
                this->_data = (T*)tmalloc(sizeof(T) * len, ALLOCTYPE_ARRAY); 
                this->_len = len; 
                memset(&_dummy, 0, sizeof(T)); 
            }
            
            array(T* data, size_t len, bool copy = false) 
            { 
                this->_len = len; 
                if (copy) { this->_data = (T*)tmalloc(sizeof(T) * len, ALLOCTYPE_ARRAY); memcpy(this->_data, data, sizeof(T) * len); }
                else { this->_data = data; }
            }

        public: 
            void dispose()
            {
                if (this->_data != NULL) { free(this->_data); }
                this->_data = NULL;
                this->_len  = 0;
            }

            void clear()
            {
                if (this->_data == NULL) { return; }
                memset(&this->_data, 0, sizeof(T) * this->_len * sizeof(T));
            }

            T& operator[](int index) { return at(index); }

        public:
            T& at(int index)
            {
                if (this->_data == NULL) { perror("Null data for array at 0x%8x", this); return _dummy; }
                if (index < 0 || index >= (int)this->_len) { perror("Index out of bounds for array at 0x%8x", this); return _dummy; }
                return this->_data[index];
            }

            T* ptr() { return this->_data; }

            size_t length() { return this->_len; }
    };
}