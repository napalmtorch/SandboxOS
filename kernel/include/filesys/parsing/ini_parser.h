#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stream.h>
#include <string.h>
#include <arraylist.h>
#include <filesys/parsing/token.h>

namespace os
{
    namespace filesystem
    {
        namespace parsing
        {
            enum class ini_objtype : uint8_t
            {
                invalid,
                l_decimal,
                l_float,
                l_char,
                l_bool,
                l_string,  
            };

            enum class ini_ast : uint8_t { base, section, object };

            class ini_ast_node
            {
                public:
                    ini_ast type;
                    ini_ast_node() { this->type = ini_ast::base; }
                    ini_ast_node(ini_ast type) { this->type = type; }
            };

            class ini_ast_section : public ini_ast_node
            {
                public:
                    std::string id;
                    std::arraylist<ini_ast_node*> nodes;
                    ini_ast_section() : ini_ast_node(ini_ast::section) { this->nodes = std::arraylist<ini_ast_node*>(); this->id = std::string(); }
            };

            class ini_ast_object : public ini_ast_node
            {
                public:
                    std::string      id;
                    void*            value;                    
                    ini_objtype      objtype;
                    ini_ast_section* section;
                    ini_ast_object(char* id, void* value, ini_objtype type) : ini_ast_node(ini_ast::object) { this->id = std::string(id); this->value = value; this->objtype = type; this->section = NULL; }
            };

            class ini_parser
            {
                public:
                    std::stream<char>       input;
                    std::arraylist<token_t> tokens;
                    std::stream<token_t>    tokstream;
                    std::arraylist<ini_ast_node*> nodes;

                private:
                    std::string _word;
                    int         _line;
                    ini_ast_section* _section;

                public:
                    ini_parser(char* fname);
                    ini_parser(std::array<char> data);                    

                public:
                    void dispose();
                    void run();

                private:
                    bool load(char* fname);
                    void tokenize();
                    void parse();

                private:
                    void handle_token();
                    void handle_newline();
                    void handle_word();
                    void handle_char();
                    void handle_string();
                    void handle_comment();

                private:
                    ini_ast_section* parse_section();
                    ini_ast_object*  parse_object();
            };
        }
    }
}