#pragma once
#include <stdint.h>
#include <stdint.h>
#include <stddef.h>
#include <hash.h>
#include <arraylist.h>

namespace std
{
    template<typename T> class dictionary
    {
        private:
            arraylist<uint32_t> _keys;
            arraylist<T>        _values;
            T                   _dummy;

        public:
            dictionary()
            {
                memset(&this->_dummy, 0, sizeof(T));
                _keys   = arraylist<uint32_t>();
                _values = arraylist<T>();
            }

            void dispose()
            {
                _keys.dispose();
                _values.dispose();
            }

            bool set(char* key, T value)
            {
                if (key == NULL || strlen(key) == 0) { perror("Invalid key while setting value in dictionary at 0x%8x", this); }
                uint32_t h = hash(key);
                int index = index_from_key(h);
                if (index < 0 || index >= _keys.length()) { _keys.add(h); _values.add(value); }
                else { _values[index] = value; }
                return true;
            }

            T& get(char* key)
            {
                int index = index_from_key(hash(key));
                validate_index(index);
                return _values[index];
            }

            int key_exists(char* key)
            {
                uint32_t h = hash(key);
                int index = index_from_key(h);
                for (size_t i = 0; i < _keys.length(); i++) { if (_keys[i] == h) { return true; } }
                return false;
            }

            int index_from_key(char* key)
            {
                uint32_t h = hash(key);
                return index_from_key(h);
            }

            int index_from_key(uint32_t hash)
            {
                for (size_t i = 0; i < _keys.length(); i++) { if (_keys[i] == hash) { return (int)i; } }
                return -1;
            }

            T& operator[](int index) { return at(index); }

            T& operator[](char* key) { return at(key); }

            T& at(int index)
            {
                if (_keys.length() == 0 || _values.length() == 0) { return _dummy; }
                validate_index(index);
                return _values[index];
            }

            T& at(char* key)
            {
                if (key == NULL || strlen(key) == 0) { return _dummy; }
                uint32_t h = hash(key);
                for (size_t i = 0; i < _keys.length(); i++)
                {
                    if (h == _keys[i]) 
                    { 
                        if (i >= _values.length()) { perror("Size mismatch for dictionary at 0x%8x", this); }
                        return _values[i]; 
                    }
                }
                return _dummy;
            }

            size_t length() { return _keys.length(); }

            arraylist<uint32_t>* keys() { return &_keys; }

            arraylist<T>* values() { return &_values; }

        private:
            void validate_index(int index)
            {
                if (index < 0 || (index >= _keys.length() || index >= _values.length())) { perror("Index out of bounds for dictionary at 0x%8x", this); }
            }
    };
}