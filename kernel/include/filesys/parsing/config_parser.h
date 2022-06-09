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
            enum class configobj_type : uint8_t
            {
                invalid,
                t_integer,
                t_float,
                t_bool,
                t_char,
                t_string,
            };

            struct config_body_t;
            struct config_object_t;

            /// @brief Structure for managing configuration file bodies
            struct config_body_t
            {
                /// @brief Pointer to name of body
                char* name;
                /// @brief List of objects within body scope
                std::arraylist<config_object_t*> objects;
            };

            /// @brief Structure for managing configuration file objects
            struct config_object_t
            {
                /// @brief Pointer to name of object
                char* name;
                /// @brief Pointer to parent body
                config_body_t* parent;
                /// @brief Type of object
                configobj_type type;
                /// @brief Size of object
                uint32_t size;
                /// @brief Raw object data
                std::array<uint8_t> data;
            };

            /// @brief Configuration file parsing utility
            class config_parser
            {
                public:
                    interpreter::tokenizer_unit   tokenizer;
                    std::arraylist<config_body_t> bodies;

                public:
                    config_parser();
                    config_parser(char* fname);

                public:
                    bool tokenize();
            };
        }
    }
}