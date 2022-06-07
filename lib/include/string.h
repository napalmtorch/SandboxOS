#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
EXTC
{
#endif

void*  memchr(const void* str, int c, size_t n);
int    memcmp(const void* str1, const char* str2, size_t n);
void*  memcpy(void* dest, const void* src, size_t n);
void*  memmove(void* dest, const void* src, size_t n);
void*  memset(void* str, int c, size_t n);
char*  strcat(char* dest, const char* src);
char*  strncat(char* dest, const char* src, size_t n);
char*  strchr(const char* str, int c);
int    strcmp(const char* str1, const char* str2);
int    strncmp(const char* str1, const char* str2, size_t n);
char*  strcpy(char* dest, const char* src);
char*  strncpy(char* dest, const char* src, size_t n);
size_t strcspn(const char* str1, const char* str2);
size_t strlen(const char* str);
char*  strpbrk(const char* str1, const char* str2);
char*  strrchr(const char* str, int c);
size_t strspn(const char* str1, const char* str2);
char*  strstr(const char* haystack, const char* needle);
char*  strtok(char* str, const char* delim);
size_t strxfrm(char* dest, const char* str, size_t n);
char*  stradd(char* str, char c);
char** strsplit(char* str, char delim, int* count);
bool   strwhite(char* str);
char*  strupper(char* str);
char*  strlower(char* str);
char*  strback(char* str);

#ifdef __cplusplus
}
#endif

namespace std
{
    class string
    {
        private:
            char*  _data;
            size_t _len;

        public:
            string();
            string(char* str);
            string(const string& str);
            string(string&& str);
            ~string();

        public:
            void dispose();
            void clear();
            void append(char c);
            void append(char* str);
            void append(const string& str);
            void append(string&& str);
            void insert(int index, char c);
            void insert(int index, char* str);
            void insert(int index, const string& str);
            void insert(string&& str);
            void backspace();
            void backspace(int count);
            void remove(int index, int count);       

        public:
            void to_upper();
            void to_lower();

        public:
            char* c_str();
            size_t length();

        public:
            string& operator=(const string& rhs);
    };
}