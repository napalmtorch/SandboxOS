#include <filesys/parsing/config_parser.h>
#include <core/kernel.h>

namespace os
{
    namespace filesystem
    {
        namespace file_parsers
        {
            config_parser::config_parser()
            {

            }

            config_parser::config_parser(char* fname)
            {
                this->tokenizer = interpreter::tokenizer_unit(fname);
                this->bodies    = std::arraylist<config_body_t>();
                
                interpreter::tokenizer_rules_t rules = (interpreter::tokenizer_rules_t)
                { 
                    .allow_decimal = true,
                    .allow_hex = true,
                    .allow_binary = true,
                    .allow_bool = true,
                    .allow_character = true,
                    .allow_string = true,
                    .allow_comment = true,
                    .allow_newline = true,
                    .allow_keywords = false,
                    .allow_typenames = false,
                    .prefix_hex = { '0', 'x', 0 },
                    .prefix_binary = { '0', 'b', 0 },
                    .prefix_comment = { '#', 0 },
                    .bool_true = { 't', 'r', 'u', 'e', 0 },
                    .bool_false = { 'f', 'a', 'l', 's', 'e', 0 }, 
                    .sym_char = '\'',
                    .sym_string = '\"',
                    .symbols = { '=', '[', ']', 0 },
                };

                this->tokenizer.set_rules(rules);
            }

            bool config_parser::tokenize()
            {
                tokenizer.run();
                return true;
            }
        }
    }
}