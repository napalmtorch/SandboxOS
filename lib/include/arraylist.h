#pragma once
#include <stdint.h>
#include <stddef.h>
#include <common.h>
#include <array.h>

namespace std
{
    template<typename T> class arraylist
    {
        private:
            T*     _items;
            size_t _len;
            T      _dummy;

        public:
            arraylist() { this->_items = nullptr; this->_len = 0; }

            arraylist(T* items, size_t len, bool copy = false)
            {
                this->_len = len;
                if (copy) { this->_items = (T*)tmalloc(sizeof(T) * len, ALLOCTYPE_ARRAY); memcpy(this->_items, items, sizeof(T) * len); }
                else { this->_items = items; }
            }

            void dispose()
            {
                if (this->_items != NULL) { free(this->_items); }
                this->_items = NULL;
                this->_len   = 0;
            }

            void clear() { dispose(); }

            void add(T item)
            {
                T* items = (T*)tmalloc(sizeof(T) * (this->_len + 1), ALLOCTYPE_ARRAY);
                if (this->_items != NULL) { memcpy(items, this->_items, sizeof(T) * this->_len); free(this->_items); }
                this->_items = items;
                this->_items[this->_len] = item;
                this->_len++;
            }

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

            T& operator[](int index) { return at(index); }

        public:
            T& at(int index)
            {
                if (this->_items == NULL) { perror("Null data for arraylist at 0x%8x", this); return _dummy; }
                if (index < 0 || index >= (int)this->_len) { perror("Index out of bounds for arraylist at 0x%8x - INDEX:%u LEN:%u", this, index, this->_len); return _dummy; }
                return this->_items[index];
            }

            T* data() { return this->_items; }

            size_t length() { return this->_len; }

            array<T> to_array() { return array<T>(this->_items, this->_len); }
    };
}