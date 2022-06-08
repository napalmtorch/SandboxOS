#pragma once
#include <stdint.h>
#include <stddef.h>
#include <common.h>
#include <array.h>

namespace std
{
    /// @brief Template class for creating dynamically-sized lists
    template<typename T> class arraylist
    {
        private:
            /// @brief Pointer to list of items
            T*     _items;
            /// @brief Amount if items in list
            size_t _len;  
            /// @brief Dummy value for returning from failed methods
            T _dummy;

        public:
            /// @brief Create a new array list
            arraylist() { this->_items = nullptr; this->_len = 0; }

            /// @brief Create an arraylist with existing properties @param data Pointer to data @param len Amount of entries @param bool Create a new copy of data
            arraylist(T* items, size_t len, bool copy = false)
            {
                this->_len = len;
                if (copy) { this->_items = (T*)tmalloc(sizeof(T) * len, ALLOCTYPE_ARRAY); memcpy(this->_items, items, sizeof(T) * len); }
                else { this->_items = items; }
            }

            /// @brief Dispose arraylist items and reset properties
            void dispose()
            {
                if (this->_items != NULL) { free(this->_items); }
                this->_items = NULL;
                this->_len   = 0;
            }

            /// @brief Clear all entries from arraylist
            void clear() { dispose(); }

            void add(T item)
            {
                T* items = (T*)tmalloc(sizeof(T) * (this->_len + 1), ALLOCTYPE_ARRAY);
                if (this->_items != NULL) { memcpy(items, this->_items, sizeof(T) * this->_len); free(this->_items); }
                this->_items = items;
                this->_items[this->_len] = item;
                this->_len++;
            }

            /// @brief Remove item in list at specified index @param index Index of item @return Successfully removed item
            bool remove_at(int index)
            {
                if (this->_len == 0 || this->_items == NULL) { return false; }
                if (this->_len == 1 && index == 0) { dispose(); return true; }

                T* items = (T*)tmalloc(sizeof(T) * (this->_len - 1), ALLOCTYPE_ARRAY);

                size_t pos = 0;
                for (size_t i = 0; i < this->_len; i++) { if (i != index) { items[pos] = this->_items[i]; pos++; } }
                free(this->_items);
                this->_items = items;
                this->_len--;
                return true;
            }

            /// @brief Access value in list by index @param index Index of item
            T& operator[](int index) { return at(index); }

        public:
            T& at(int index)
            {
                if (this->_items == NULL) { perror("Null data for arraylist at 0x%8x", this); return _dummy; }
                if (index < 0 || index >= (int)this->_len) { perror("Index out of bounds for arraylist at 0x%8x - INDEX:%u LEN:%u", this, index, this->_len); return _dummy; }
                return this->_items[index];
            }

            /// @brief Get pointer to data list @return Pointer to data
            T* ptr() { return this->_items; }

            /// @brief Get amount of entries
            size_t length() { return this->_len; }

            /// @brief Return an array representation of this list @return Array containing data with matching list pointer
            array<T> to_array() { return array<T>(this->_items, this->_len); }
    };
}