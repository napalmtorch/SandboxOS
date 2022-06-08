#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <arraylist.h>
#include <stream.h>

namespace os
{
    namespace interpreter
    {
        /// @brief Types of tokens
        enum class token_type : uint8_t
        {
            /// @brief Invalid token
            invalid,
            /// @brief Identifier/name
            identifier,
            /// @brief Keyword
            keyword,
            /// @brief Typename
            type_name,
            /// @brief Symbol
            symbol,
            /// @brief Newline feed
            newline,
            /// @brief Literal decimal value
            literal_dec,
            /// @brief Literal hexadecimal value
            literal_hex,
            /// @brief Literal binary value
            literal_binary,
            /// @brief Literal boolean value
            literal_bool,
            /// @brief Literal character value
            literal_char,
            /// @brief Literal string value
            literal_string,
        };

        /// @brief Structure for managing language syntax tokens
        struct token_t
        {
            public:
                /// @brief Type of token
                token_type type;
                /// @brief Line containing token
                int line;
                /// @brief Pointer to token lexeme
                char* value;

                /// @brief Default token constructor - initialized with null values
                token_t() { this->type = token_type::invalid; this->line = 1; this->value = NULL; }
                /// @brief Create new token @param type Type of token @param line Line containing token @param value Pointer to token lexeme
                token_t(token_type type, int line, char* value) { this->type = type; this->line = line; this->value = value; }
        };

        /// @brief Structure for managing tokenization rules
        typedef struct
        {
            /// @brief Allow literal decimals values
            bool allow_decimal : 1;
            /// @brief Alow literal hexadecimal values
            bool allow_hex : 1;
            /// @brief Allow literal binary values
            bool allow_binary : 1;
            /// @brief Allow literal boolean values
            bool allow_bool : 1;
            /// @brief Allow literal character values
            bool allow_character : 1;
            /// @brief Allow literal string values
            bool allow_string : 1;
            /// @brief Allow comments
            bool allow_comment : 1;
            /// @brief Allow newline tokens
            bool allow_newline : 1;
            /// @brief Allow keywords
            bool allow_keywords : 1;
            /// @brief Allow typenames
            bool allow_typenames : 1;
            /// @brief Prefix value for hexadecimal literals - i.e. '0x', 'h'
            char prefix_hex[16];
            /// @brief Prefix value for binary literals - i.e. '0b', 'b'
            char prefix_binary[16];
            /// @brief Prefix value for comments - i.e. '//' ';' '#'
            char prefix_comment[16];
            /// @brief Value for literal boolean(true)
            char bool_true[32];
            /// @brief Value for literal boolean(false)
            char bool_false[32];
            /// @brief Symbol used for containing character literals
            char sym_char;
            /// @brief Symbol used for containing string literals
            char sym_string;         
            /// @brief List of accepted symbols
            char symbols[64];
            /// @brief List of accepted keywords
            std::arraylist<char*> keywords;
            /// @brief List of accepted typenames
            std::arraylist<char*> typenames;
        } tokenizer_rules_t;

        /// @brief Dynamic language syntax tokenization unit
        class tokenizer_unit
        {
            public:
                /// @brief Input stream of characters - source code
                std::stream<char> input;
                /// @brief Output tokens
                std::arraylist<token_t> tokens;
                /// @brief Tokenizer rules
                tokenizer_rules_t rules;

            private:
                /// @brief Current line
                int _line;
                /// @brief Current word
                char* _word;
                /// @brief Length of current word
                size_t _wordlen;

            public:
                /// @brief Create tokenizer with specified input data @param input Input characters
                tokenizer_unit(std::array<char> in);
                /// @brief Create tokenizer with specified input data @param input Input characters
                tokenizer_unit(std::array<uint8_t> in);
                /// @brief Load input data from file @param fname Pointer to filename string
                tokenizer_unit(char* fname);
            
            public:
                /// @brief Dispose all allocations created by this
                void dispose();
                /// @brief Load data from array into input stream
                void load(std::array<char> in);
                /// @brief Set tokenization rules @param rules Rules to follow while tokenizing input
                void set_rules(tokenizer_rules_t rules);
                /// @brief Run tokenization process on input data
                void run();

            private:
                /// @brief Add token to token list @param type Type of token @param Line containing token @param value Pointer to lexeme @param copy Copy lexeme value to new string pointer
                void add_token(token_type type, int line, char* value, bool copy = true);
                /// @brief Clear temporary word value
                void clear_word();

            public:
                /// @brief Handle input character
                void handle(char c);
                /// @brief Handle newline feed
                void handle_newline();
                /// @brief Handle string of characters
                void handle_word();
                /// @brief Handle character literal
                void handle_char_literal();
                /// @brief Handle string literal
                void handle_str_literal();
                /// @brief Handle comment
                void handle_comment();

            public:
                /// @brief Determine if string is a decimal literal @param value Pointer to string @return String is decimal
                bool is_literal_dec(char* value);
                /// @brief Determine if string is a hexadecimal literal @param value Pointer to string @return String is hexadecimal
                bool is_literal_hex(char* value);
                /// @brief Determine if string is a binary literal @param value Pointer to string @return String is binary
                bool is_literal_bin(char* value);
                /// @brief Determine if string is a boolean literal @param value Pointer to string @return String is boolean
                bool is_literal_bool(char* value);
                /// @brief Determine if string is a keyword @param value Pointer to string @return String is keyword
                bool is_keyword(char* value);
                /// @brief Determine if string is a typename @param value Pointer to string @return String is typename
                bool is_typename(char* value);
                /// @brief Deterine if character is a symbol @param value Character value @return Character is symbol
                bool is_symbol(char c);
                /// @brief Determine if string is a symbol @param value Pointer to string @return String is symbol
                bool is_symbol(char* value);

            public:
                /// @brief Get string representation of token type @param type Type of token @return Pointer to type string
                static const char* typestr(token_type type);
        };
    }
}