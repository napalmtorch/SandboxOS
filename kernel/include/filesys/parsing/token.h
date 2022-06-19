#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <arraylist.h>
#include <core/exec/interpreter/tokenizer.h>

namespace os
{
    namespace filesystem
    {
        namespace parsing
        {
            enum class token_type : uint8_t
            {
                invalid,
                identifier,
                keyword,
                newline,
                
                equals,
                l_bracket,
                r_bracket,

                literal_dec,
                literal_hex,
                literal_bin,
                literal_float,
                literal_bool,
                literal_char,
                literal_string,
            };

            struct token_t
            {
                public:
                    token_type type;
                    int        line;
                    char*      value;  
                    token_t() { this->type = token_type::invalid; this->line = 0; this->value = NULL; }
                    token_t(token_type type, int line, char* value, bool copy = true)
                    {
                        this->type = type;
                        this->line = line;
                        if (!copy) { this->value = value; return; }
                        this->value = (char*)tmalloc(strlen(value) + 1, ALLOCTYPE_STRING);
                        strcpy(this->value, value);
                    }
            };
        }
    }
}