#include <core/exec/interpreter/tokenizer.h>
#include <core/kernel.h>

namespace os
{
    namespace interpreter
    {
        tokenizer_unit::tokenizer_unit(std::array<char> in)
        {
            load(in);
        }

        tokenizer_unit::tokenizer_unit(std::array<uint8_t> in)
        {
            load(std::array<char>((char*)in.ptr(), in.length(), false));
        }

        tokenizer_unit::tokenizer_unit(char* fname)
        {
            // attempt to load file
            file_t* file = filesystem::open_file(fname, "r");
            if (file == NULL) { perror("Tokenizer failed to locate file at '%s'", fname); return; }

            // populate input stream with file data and close file
            input = std::stream<char>((char*)file->data.ptr(), file->data.length(), true);
            filesystem::close_file(file);

            // reset values
            tokens.clear();
            _wordlen = input.length() + 1;
            _word = (char*)tmalloc(_wordlen, ALLOCTYPE_STRING);
            _line = 1;
            
            // finished
            printf("%s Tokenizer loaded file at '%s'\n", DEBUG_OK, fname);
        }

        void tokenizer_unit::dispose()
        {
            if (input.ptr() != NULL) { input.dispose(); }
            for (size_t i = 0; i < tokens.length(); i++) { free(tokens[i].value); }
            tokens.dispose();
        }

        void tokenizer_unit::load(std::array<char> in)
        {
            // populate input stream with data
            this->input = std::stream<char>(in, true);

            // reset values
            tokens.clear();
            _wordlen = input.length() + 1;
            _word = (char*)tmalloc(_wordlen, ALLOCTYPE_STRING);
            _line = 1;
            
            // finished
            printf("%s Tokenizer loaded data at 0x%8x\n", DEBUG_OK, input.ptr());
        }

        void tokenizer_unit::set_rules(tokenizer_rules_t rules)
        {
            this->rules = rules;
            printf("%s Set tokenizer rules to structure at 0x%8x\n", &rules);
        }

        void tokenizer_unit::run()
        {
            while (!input.done())
            {
                char c = input.next();
                handle(c);
            }
            handle_word();
            printf("%s Tokenizer finished - %u tokens created\n", DEBUG_OK, tokens.length());
        }

        void tokenizer_unit::add_token(token_type type, int line, char* value, bool copy)
        {
            token_t tok;
            if (!copy) { tok = token_t(type, line, value); }
            else
            {
                char* str = (char*)tmalloc(strlen(value) + 1, ALLOCTYPE_STRING);
                strcpy(str, value);
                tok = token_t(type, line, str);
            }
            tokens.add(tok);
            printf("%s Created token - LINE:%u TYPE:%s VALUE:%s\n", DEBUG_INFO, line, typestr(type), tok.value);
        }

        void tokenizer_unit::clear_word()
        {
            if (_word == NULL) { return; }
            memset(_word, 0, _wordlen);
        }

        void tokenizer_unit::handle(char c)
        {
            char cstr[2] = { c, 0 };

            if (is_symbol(c)) { handle_word(); add_token(token_type::symbol, _line, cstr, true); clear_word(); return; }

            if (c == rules.sym_char && rules.allow_character) { handle_char_literal(); clear_word(); return; }
            if (c == rules.sym_string && rules.allow_string)  { handle_str_literal();  clear_word(); return; }

            switch (c)
            {
                default: { stradd(_word, c); break; }
                case '\r': { break; }
                case '\t': { break; }
                case '\n': { handle_word(); add_token(token_type::newline, _line++, "NL", true); break; }
                case ' ': { handle_word(); break; }
            }
        }

        void tokenizer_unit::handle_word()
        {
            if (strlen(_word) == 0) { return; }
            
            if (is_keyword(_word) && rules.allow_keywords) { add_token(token_type::keyword, _line, _word, true); }
            else if (is_typename(_word) && rules.allow_typenames) { add_token(token_type::type_name, _line, _word, true); }
            else if (is_literal_dec(_word) && rules.allow_decimal) { add_token(token_type::literal_dec, _line, _word, true); }
            else if (is_literal_hex(_word) && rules.allow_hex) { add_token(token_type::literal_hex, _line, _word, true); }
            else if (is_literal_bin(_word) && rules.allow_binary) { add_token(token_type::literal_binary, _line, _word, true); }
            else if (is_literal_bool(_word) && rules.allow_bool) { add_token(token_type::literal_bool, _line, _word, true); }
            else { add_token(token_type::identifier, _line, _word, true); }
            clear_word();
        }

        void tokenizer_unit::handle_char_literal()
        {
            std::string value;
            while (!input.done())
            {
                char c = input.next();
                if (c == 0) { perror("Unterminated char literal at line %u", _line); return; }
                if (c == rules.sym_char) { break; }
                value.append(c);
            }
            add_token(token_type::literal_char, _line, value.c_str(), true);
            value.dispose();
        }

        void tokenizer_unit::handle_str_literal()
        {
            std::string value;
            while (!input.done())
            {
                char c = input.next();
                if (c == 0) { perror("Unterminated char literal at line %u", _line); return; }
                if (c == rules.sym_string) { break; }
                value.append(c);
            }
            add_token(token_type::literal_string, _line, value.c_str(), true);
            value.dispose();
        }

        void tokenizer_unit::handle_comment()
        {
            
        }

        bool tokenizer_unit::is_literal_dec(char* value)
        {
            if (value == NULL || strlen(value) == 0) { return false; }

            int i = 0;
            while (value[i] != 0) { if (!isdigit(value[i])) { return false; } i++; }
            return true;
        }

        bool tokenizer_unit::is_literal_hex(char* value)
        {
            if (value == NULL || strlen(value) == 0) { return false; }
            if (strncmp(value, rules.prefix_hex, strlen(rules.prefix_hex))) { return false; }
            
            int i = strlen(rules.prefix_hex);
            while (value[i] != 0) { if (!isxdigit(value[i])) { return false; } i++; }
            return true;
        }

        bool tokenizer_unit::is_literal_bin(char* value)
        {
            if (value == NULL || strlen(value) == 0) { return false; }
            if (!memcmp(value, rules.prefix_binary, strlen(rules.prefix_binary))) { return false; }        
            
            int i = strlen(rules.prefix_binary);
            while (value[i] != 0) { if (value[i] != '0' && value[i] != '1') { return false; } i++; }
            return true;
        }

        bool tokenizer_unit::is_literal_bool(char* value)
        {
            if (value == NULL || strlen(value) == 0) { return false; }
            if (!strcmp(value, rules.bool_false)) { return true; }
            if (!strcmp(value, rules.bool_true)) { return true; }
            return false;
        }

        bool tokenizer_unit::is_keyword(char* value)
        {
            if (value == NULL || strlen(value) == 0) { return false; }
            for (size_t i = 0; i < rules.keywords.length(); i++)
            {
                if (!strcmp(rules.keywords[i], value)) { return true; }
            }
            return false;
        }

        bool tokenizer_unit::is_typename(char* value)
        {
            if (value == NULL || strlen(value) == 0) { return false; }
            for (size_t i = 0; i < rules.typenames.length(); i++)
            {
                if (!strcmp(rules.keywords[i], value)) { return true; }
            }
            return false;
        }

        bool tokenizer_unit::is_symbol(char c)
        {
            for (size_t i = 0; i < strlen(rules.symbols); i++)
            {
                if (rules.symbols[i] == c) { return true; }
            }
            return false;
        }

        bool tokenizer_unit::is_symbol(char* value)
        {
            if (value == NULL || strlen(value) == 0) { return false; }
            if (strlen(value) != 1) { return false; }
            return is_symbol(value[0]);
        }

        const char* tokenizer_unit::typestr(token_type type)
        {
            switch (type)
            {
                default:                         { return "invalid "; }
                case token_type::identifier:     { return "ident   "; }
                case token_type::keyword:        { return "keyword "; }
                case token_type::type_name:      { return "typename"; }
                case token_type::symbol:         { return "symbol  "; }
                case token_type::newline:        { return "newline"; }
                case token_type::literal_dec:    { return "decimal "; }
                case token_type::literal_hex:    { return "hex     "; }
                case token_type::literal_binary: { return "binary  "; }
                case token_type::literal_bool:   { return "boolean "; }
                case token_type::literal_char:   { return "char    "; }
                case token_type::literal_string: { return "string  "; }
            }
        }
    }
}