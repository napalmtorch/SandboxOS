#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <common.h>

namespace std
{
    /// @brief Template class for storing array types
    template<typename T> class array
    {
        private:
            /// @brief Pointer to list of data
            T* _data;
            /// @brief Amount if items in list
            size_t _len;  
            /// @brief Dummy value for returning from failed methods
            T _dummy;

        public:
            /// @brief Default array constructor - initializes with null values
            array() { this->_data = NULL; this->_len = 0; }
            
            /// @brief Create a new array with specified size @param len Amount of entries
            array(size_t len) 
            { 
                if (len == 0) { this->_data = NULL; this->_len = 0; return; }
                this->_data = (T*)tmalloc(sizeof(T) * len, ALLOCTYPE_ARRAY); 
                this->_len = len; 
                memset(&_dummy, 0, sizeof(T)); 
            }
            
            /// @brief Create an array with existing properties @param data Pointer to data @param len Amount of entries @param bool Create a new copy of data
            array(T* data, size_t len, bool copy = false) 
            { 
                this->_len = len; 
                if (copy) { this->_data = (T*)tmalloc(sizeof(T) * len, ALLOCTYPE_ARRAY); memcpy(this->_data, data, sizeof(T) * len); }
                else { this->_data = data; }
            }

        public: 
            /// @brief Dispose array data and reset properties
            void dispose()
            {
                if (this->_data != NULL) { free(this->_data); }
                this->_data = NULL;
                this->_len  = 0;
            }

            /// @brief Clear all entries from array
            void clear()
            {
                if (this->_data == NULL) { return; }
                memset(&this->_data, 0, sizeof(T) * this->_len * sizeof(T));
            }

            /// @brief Access value in list by index @param index Index of item
            T& operator[](int index) { return at(index); }

        public:
            /// @brief Access value in list by index @param index Index of item @return Reference to item
            T& at(int index)
            {
                if (this->_data == NULL) { perror("Null data for array at 0x%8x", this); return _dummy; }
                if (index < 0 || index >= (int)this->_len) { perror("Index out of bounds for array at 0x%8x", this); return _dummy; }
                return this->_data[index];
            }

            /// @brief Get pointer to data list @return Pointer to data
            T* ptr() { return this->_data; }

            /// @brief Get amount of entries
            size_t length() { return this->_len; }
    };
}