#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

#ifdef __cplusplus
EXTC
{
#endif

void* memchr(const void* str, int c, size_t n)
{
    uint8_t* p    = (uint8_t*)str;
    uint8_t* find = NULL;
    while((str != NULL) && (n--))
    {
        if( *p != (uint8_t)c) { p++; }
        else { find = p; break; }
    }
    return find;
}

int memcmp(const void* str1, const void* str2, size_t size)
{
    int r = 0;
    uint8_t* x = (uint8_t*)str1;
    uint8_t* y = (uint8_t*)str2;

    while (size--) 
    {
        r = *x - *y;
        if (r) { return r; }
        x++; y++;
    }
    return 0;
}

void* memcpy(void* dest, const void* src, size_t size)
{
    if (size % 4 == 0)
    {
        size_t l = size / 4;
        asm volatile("cld;rep movsl" : "+D"(dest), "+S"(src), "+c"(l) : : "memory");
        return dest;
    }
    if (size % 2 == 0)
    {
        size_t l = size / 2;
        asm volatile("cld;rep movsw" : "+D"(dest), "+S"(src), "+c"(l) : : "memory");
        return dest;
    }
    asm volatile("cld;rep movsb" : "+D"(dest), "+S"(src), "+c"(size) : : "memory");
    return dest;
}

void* memset(void* dest, int data, size_t size)
{
    if (size % 4 == 0)
    {     
        size_t l = size / 4;
        asm volatile("cld;rep stosl" : "+D"(dest), "+c"(l) : "a"(data) : "memory");
        return dest;
    }
    if (size % 2 == 0)
    {
        size_t l = size / 2;
        asm volatile("cld;rep stosw" : "+D"(dest), "+c"(l) : "a"(data) : "memory");
        return dest;
    }
    asm volatile("cld;rep stosb" : "+D"(dest), "+c"(size) : "a"(data) : "memory");
    return dest;
}

void* memmove(void* dest, const void* src, size_t n)
{
    return NULL;
}

char* strcat(char* dest, const char* src)
{
    if (dest == NULL) { return NULL; }
    if (src == NULL) { return dest; }
    uint64_t src_len = strlen(src);
    uint64_t dest_len = strlen(dest);
    for (uint64_t i = 0; i < src_len; i++) { dest[dest_len + i] = src[i]; }
    dest[strlen(dest)] = 0;
    return dest;
}

char* strncat(char* dest, const char* src, size_t n)
{
    if (dest == NULL) { return NULL; }
    if (src == NULL) { return dest; }
    uint64_t dest_len = strlen(dest);
    for (uint64_t i = 0; i < n; i++) { dest[dest_len + i] = src[i]; }
    dest[strlen(dest)] = 0;
    return dest;
}

char* strchr(const char* str, int c)
{
    do 
    {
        if (*str == c) { return (char*)str; }
    } while (*str++);
    return NULL;
}

int strcmp(const char* str1, const char* str2)
{
    if (str1 == NULL && str1 != str2) { return 1; }
    while (*str1)
    {
        if (*str1 != *str2) { break; }
        str1++; str2++;
    }
    return *(char*)str1 - *(char*)str2;
}

int strncmp(const char* str1, const char* str2, size_t n)
{
    if (str1 == NULL || str2 == NULL) { return 1; }
    while (n && *str1 && (*str1 == *str2))
    {
        ++str1;
        ++str2;
        --n;
    }
    if (n == 0) { return 0; }
    else { return (*(char*)str1 = *(char*)str2); }
    return 0;
}

char* strcpy(char* dest, const char* src)
{
    if (dest == NULL) { return NULL; }
    if (src == NULL) { return NULL; }

    int i = 0;
    while (src[i] != 0) { dest[i] = src[i]; i++; }
    return dest;
}

char* strncpy(char* dest, const char* src, size_t n)
{
    for (uint32_t i = 0; i < n; i++) { dest[i] = src[i]; }
    return dest;
}

size_t strcspn(const char* str1, const char* str2)
{
    return 0;
}

size_t strlen(const char* str)
{
    if (str == NULL) { return 0; }
    uint32_t len = 0;
    while (str[len] != 0) { len++; }
    return len;
}

char* strpbrk(const char* str1, const char* str2)
{
    return NULL;
}

char* strrchr(const char* str, int c)
{
    return NULL;
}

size_t strspn(const char* str1, const char* str2)
{
    return 0;
}

char* strstr(const char* haystack, const char* needle)
{
    return NULL;
}

char* strtok(char* str, const char* delim)
{
    return NULL;
}

size_t strxfrm(char* dest, const char* str, size_t n)
{
    return 0;
}

char* stradd(char* str, char c)
{
    if (str == NULL) { return NULL; }
    uint64_t len = strlen(str);
    str[len] = (char)c;
    str[len + 1] = 0;
    return str;
}

char** strsplit(char* str, char delim, int* count)
{
    if (str == NULL) { return NULL; }
    if (strlen(str) == 0) { return NULL; }

    int len = strlen(str);
    uint32_t num_delimeters = 0;

    for(int i = 0; i < len - 1; i++) { if(str[i] == delim) { num_delimeters++; } }

    uint32_t arr_size = sizeof(char*) * (num_delimeters + 1);
    char** str_array = (char**)tmalloc(arr_size, ALLOCTYPE_STRING);
    int str_offset = 0;

    int start = 0;
    int end = 0;
    while(end < len)
    {
        while(str[end] != delim && end < len) { end++; }

        char* substr = (char*)tmalloc(end - start + 1, ALLOCTYPE_STRING);
        memcpy(substr, str + start, end - start);
        start = end + 1;
        end++;
        str_array[str_offset] = substr;
        str_offset++;
    }
    *count = str_offset;
    return str_array;
}

bool strwhite(char* str)
{
    if (str == NULL) { return false; }
    if (strlen(str) == 0) { return false; }
    for (int i = 0; i < strlen(str); i++)
    {
        if (!isspace(str[i])) { return false; }
    }
    return true;
}

char* strupper(char* str)
{
    for (int i = 0; i < strlen(str); i++) { str[i] = toupper(str[i]); }
    return str;
}

char* strlower(char* str)
{
    for (int i = 0; i < strlen(str); i++) { str[i] = tolower(str[i]); }
    return str;
}

char* strback(char* str)
{
    if (str == NULL) { return NULL; }
    str[strlen(str) - 1] = 0;
    return str;
}

#ifdef __cplusplus
}
#endif


namespace std
{
    std::string to_string(bool value)
    {
        if (value) { return "true"; }
        return "false";
    }
    
    std::string to_string(int32_t value)
    {
        char temp[32];
        memset(temp, 0, sizeof(temp));
        itoa(value, temp, 10);
        return std::string(temp);
    }
    
    std::string to_string(uint32_t value)
    {
        char temp[32];
        memset(temp, 0, sizeof(temp));
        ltoa(value, temp, 10);
        return std::string(temp);
    }

    std::string to_string(double value)
    {
        char temp[32];
        memset(temp, 0, sizeof(temp));
        ftoa(value, temp, 4);
        if (value < 1)
        {
            char temp2[32];
            memset(temp2, 0, sizeof(temp2));
            stradd(temp2, '0');
            strcat(temp2, temp);
            while (temp2[strlen(temp2) - 1] == '0') { strback(temp2); }
            return std::string(temp2);
        }
        while (temp[strlen(temp) - 1] == '0') { strback(temp); }
        return std::string(temp);
    }

    // --------------------------------------------------------------------------------------------------------------

    string::string() { this->_data = NULL; this->_len = 0; }

    string::string(char* str)
    {
        if (str == NULL || strlen(str) == 0) { this->_data = NULL; this->_len = 0; return; }
        this->_len = strlen(str);
        this->_data = (char*)tmalloc(this->_len + 1, ALLOCTYPE_ARRAY);
        strcpy(this->_data, str);
    }

    string::string(const string& str)
    {
        if (&str == NULL) { this->_data = NULL; this->_len = 0; return; }
        if (str._len == 0) { this->_data = NULL; this->_len = 0; return; }

        this->_len  = str._len;
        this->_data = (char*)tmalloc(str._len, ALLOCTYPE_ARRAY);
        strcpy(this->_data, str._data);
    }

    string::string(string&& str)
    {
        if (&str == NULL) { this->_data = NULL; this->_len = 0; return; }
        if (str._len == 0) { this->_data = NULL; this->_len = 0; return; }

        this->_len  = str._len;
        this->_data = (char*)tmalloc(str._len, ALLOCTYPE_ARRAY);
        strcpy(this->_data, str._data);
    }

    string::~string() { dispose(); }

    void string::dispose()
    {
        if (this->_data != NULL) { free(this->_data); this->_data = NULL; } 
        this->_len = 0; 
    }

    void string::clear() { dispose(); }

    void string::append(char c)
    {
        char* data = (char*)tmalloc(this->_len + 2, ALLOCTYPE_ARRAY);

        if (this->_data != NULL)
        {
            memcpy(data, this->_data, this->_len);
            free(this->_data);
        }

        this->_data = data;
        stradd(this->_data, c);
        this->_len = strlen(this->_data);
    }

    void string::append(char* str)
    {
        if (str == NULL || strlen(str) == 0) { return; }
        char* data = (char*)tmalloc(this->_len + strlen(str) + 1, ALLOCTYPE_ARRAY);

        if (this->_data != NULL)
        {
            memcpy(data, this->_data, this->_len);
            free(this->_data);
        }

        this->_data = data;
        strcat(this->_data, str);
        this->_len = strlen(this->_data);
    }

    void string::append(const string& str) { append(str._data); }

    void string::append(string&& str) { append(str._data); }

    void string::insert(int index, char c)
    {
        
    }

    void string::insert(int index, char* str)
    {

    }

    void string::insert(int index, const string& str)
    {

    }

    void string::insert(string&& str)
    {

    }

    void string::backspace()
    {

    }

    void string::backspace(int count)
    {

    }

    void string::remove(int index, int count)
    {

    }

    void string::to_upper()
    {

    }

    void string::to_lower()
    {

    }

    char* string::c_str() { return this->_data; }

    size_t string::length() { return this->_len; }

    string& string::operator=(const string& rhs)
    {
        if (this == &rhs) { return *this; }
        if (rhs._data == NULL || rhs._len == 0) { if (_data != NULL) { free(_data); } _len = 0; return *this; }
        if (_data != NULL) { free(_data); }
        _data = (char*)tmalloc(rhs._len + 1, ALLOCTYPE_ARRAY);
        _len  = strlen(_data);
        strcpy(_data, rhs._data);
        return *this;
    }
}
