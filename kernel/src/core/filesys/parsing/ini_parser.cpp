#include <filesys/parsing/ini_parser.h>
#include <core/kernel.h>

namespace os
{
    namespace filesystem
    {
        namespace parsing
        {
            ini_parser::ini_parser(char* fname)
            {
                if (!load(fname)) { perror("INI parser failed to locate file at '%s'", fname); return; }
            }
        
            ini_parser::ini_parser(std::array<char> data)
            {

            }

            void ini_parser::dispose()
            {

            }

            void ini_parser::run()
            {
                tokenize();

                for (size_t i = 0; i < tokens.length(); i++)
                {
                    printf("%s TOKEN - LINE:%u TYPE:0x%2x VAL:%s\n", DEBUG_INFO, tokens[i].line, tokens[i].type, tokens[i].value);
                }

                parse();
            }

            void ini_parser::tokenize()
            {
                _word = std::string();
                _line = 1;
                while (!input.done())
                {
                    handle_token();
                }
                handle_word();
            }

            void ini_parser::parse()
            {
                tokstream = std::stream<token_t>(tokens.ptr(), tokens.length(), false);

                while (!tokstream.done())
                {
                    token_t tok = tokstream.next();
                    if (tok.type == token_type::l_bracket)
                    {
                        ini_ast_section* section = parse_section();
                        _section = section;
                        nodes.add(section);
                        printf("%s Parsed section - ID:%s NODES:%u\n", DEBUG_INFO, section->id.c_str(), section->nodes.length());
                    }
                    else if (tok.type == token_type::identifier)
                    {
                        ini_ast_object* obj = parse_object();
                        if (_section != NULL) { _section->nodes.add(obj); obj->section = _section; }
                        else { nodes.add(obj); }
                        printf("%s Parsed object - ID:%s TYPE:0x%2x DATA:0x%8x SECTION:%s\n", DEBUG_INFO, obj->id.c_str(), obj->objtype, obj->value, (obj->section == NULL ? "" : obj->section->id));
                    }
                }
            }

            bool ini_parser::load(char* fname)
            {
                file_t* file = open_file(fname, "r");
                if (file == NULL) { return false; }
                input = std::stream<char>((char*)file->data.ptr(), file->data.length(), true);
                close_file(file);

                printf("%s INI parser loaded file at '%s'\n", DEBUG_OK, fname);
                return true;
            }

            void ini_parser::handle_token()
            {
                char c = input.next();
                if (c == '\r' || c == '\t') { return; }

                switch (c)
                {
                    default:   { _word.append(c);  break; }
                    case '\n': { handle_newline(); break; }
                    case ' ':  { handle_word();    break; }
                    case '\'': { handle_char();    break; }
                    case '\"': { handle_string();  break; }
                    case '[':  { handle_word(); tokens.add(token_t(token_type::l_bracket, _line, "[", true)); _word.clear(); break; }
                    case ']':  { handle_word(); tokens.add(token_t(token_type::r_bracket, _line, "]", true)); _word.clear(); break; }
                    case '=':  { handle_word(); tokens.add(token_t(token_type::equals,    _line, "=", true)); _word.clear(); break; }
                }
            }

            void ini_parser::handle_newline()
            {
                handle_word();
                if (tokens[tokens.length() - 1].type != token_type::newline) { tokens.add(token_t(token_type::newline, _line, "NL", true)); }
                _line++;
                _word.clear();
            }

            void ini_parser::handle_word()
            {
                if (_word.length() == 0) { return; }
                tokens.add(token_t(token_type::identifier, _line, _word.c_str(), true));
                _word.clear();
            }

            void ini_parser::handle_char()
            {
                std::string str;
                while (true)
                {
                    char c = input.next();
                    if (c == 0) { perror("Unterminated char literal at line %u", _line); return; }
                    if (c == '\'') { break; }
                    str.append(c);
                }
                tokens.add(token_t(token_type::literal_char, _line, str.c_str(), true));
                str.dispose();
            }

            void ini_parser::handle_string()
            {
                std::string str;
                while (true)
                {
                    char c = input.next();
                    if (c == 0) { perror("Unterminated string literal at line %u", _line); return; }
                    if (c == '\"') { break; }
                    str.append(c);
                }
                tokens.add(token_t(token_type::literal_string, _line, str.c_str(), true));
                str.dispose();
            }

            void ini_parser::handle_comment()
            {

            }

            ini_ast_section* ini_parser::parse_section()
            {
                ini_ast_section* section = new ini_ast_section();

                token_t tok_id = tokstream.next();
                if (tok_id.type != token_type::identifier) { perror("Expected identifier for section at line %u", tok_id.line); }
                section->id = std::string(tok_id.value);

                token_t tok_rbracket = tokstream.next();
                if (tok_rbracket.type != token_type::r_bracket) { perror("Expected ']' after section at line %u", tok_rbracket.line); }

                return section;
            }

            ini_ast_object* ini_parser::parse_object()
            {
                ini_ast_object* obj = new ini_ast_object("", NULL, ini_objtype::invalid);

                token_t tok_id = tokstream.next();
                printf("TOK - %s\n", tok_id.value);
                if (tok_id.type != token_type::identifier) { perror("Expected identifier for object at line %u", tok_id.line); }
                obj->id = std::string(tok_id.value);

                token_t tok_eql = tokstream.next();
                if (tok_eql.type != token_type::equals) { perror("Expected '=' for object at line %u", tok_eql.line); }

                token_t tok_val = tokstream.next();
                if (tok_val.type < token_type::literal_dec) { perror("Expected literal value for object at line %u", tok_val.line); }

                return obj;
            }
        }
    }
}