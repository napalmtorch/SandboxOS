#pragma once

#include <array.h>
#include <stream.h>
#include <arraylist.h>
#include <stdint.h>

namespace os
{
    /// @brief C++ namespace for the language runtime.
    namespace runtime
    {
        /// @brief enum for the different types of statements.
        enum class statement_type: uint8_t
        {
            /// @brief The statement is a null statement.
            null,

            /// @brief The statement is a runtime directive.
            directive,
            
            /// @brief The statement is a declaration.
            declaration,
            /// @brief The statement is an expression.
            expression,
            /// @brief The statement is an attribute.
            attribute,
            /// @brief The statement is a literal.
            literal,
            /// @brief The statement is an operand.
            operand,
            /// @brief The statement is a block.
            block
        };
        /// @brief class representing a statement.
        class statement_t
        {
        private:
            /// @brief the type of the statement.
            statement_type _type;
            /// @brief the statement's attributes.
            std::arraylist<statement_t*> _attributes;
            /// @brief the statement's children.
            std::arraylist<statement_t*> _children;
            /// @brief internal function to dispose the statement.
            inline virtual void internal_dispose()
            {
            }
        public:
            /// @brief constructor.
            inline statement_t(statement_type type)
            {
                _type = type;
            }
            /// @brief destructor.
            inline ~statement_t()
            {
            }

        public:
            /// @brief get the type of the statement.
            inline statement_type type() const
            {
                return _type;
            }
            /// @brief get the statement's attributes.
            virtual inline std::arraylist<statement_t*> attributes() const
            {
                return _attributes;
            }
            /// @brief get the statement's children.
            virtual inline std::arraylist<statement_t*> children() const
            {
                return _children;
            }
        public:
            /// @brief add an attribute to the statement.
            inline virtual void add_attribute(statement_t* attribute)
            {
                _attributes.add(attribute);
            }
            /// @brief add a child to the statement.
            inline virtual void add_child(statement_t* child)
            {
                _children.add(child);
            }
        public:
            inline void dispose()
            {
                uint32_t i;
                // dispose attributes
                for(i = 0; i < _attributes.length(); i++)
                {
                    _attributes.dispose();
                    delete _attributes[i];
                }
                // dispose children
                for(i = 0; i < _children.length(); i++)
                {
                    _children.dispose();
                    delete _children[i];
                }
                _attributes.dispose();
                _children.dispose();

                // dispose this statement
                _type = statement_type::null;
                internal_dispose();
            }
        };
        /// @brief class representing a runtime directive.
        class directive_t: public statement_t
        {
        private:
            /// @brief the directive's name.
            std::string _name;
            /// @brief internal function to dispose the statement.
            inline void internal_dispose() override
            {
                _name.dispose();
            }
        public:
            /// @brief constructor.
            inline directive_t(std::string name)
            : statement_t(statement_type::directive) 
            {
                _name = name;
            }
            /// @brief destructor.
            inline ~directive_t()
            {
            }
        public:
            /// @brief get the directive's name.
            inline std::string name() const
            {
                return _name;
            }
        public:
            inline std::arraylist<statement_t*> attributes() const override
            {
                return std::arraylist<statement_t*>();
            }
        };
        /// @brief class representing a declaration.
        class declaration_t: public statement_t
        {
        public:
            enum class declaration_type: uint8_t
            {
                /// @brief the declaration is a variable.
                variable,
                /// @brief the declaration is a function.
                function
            };
        private:
            /// @brief the declaration's type.
            declaration_type _type;
            /// @brief internal function to dispose the statement.
            virtual void internal_dispose() = 0;
        public:
            /// @brief constructor.
            inline declaration_t(declaration_type type)
            : statement_t(statement_type::declaration)
            {
                _type = type;
            }
            /// @brief destructor.
            inline ~declaration_t()
            {
            }
        public:
            /// @brief get the declaration's type.
            inline declaration_type declaration_type() const
            {
                return _type;
            }
        };
        /// @brief class representing a variable declaration.
        class variable_declaration_t: public declaration_t
        {
        private:
            /// @brief the variable's name.
            std::string _name;
            /// @brief the variable's type.
            std::string _type;
            /// @brief the variable's value.
            statement_t* _value;
            /// @brief internal function to dispose the statement.
            void internal_dispose() override
            {
                _name.dispose();
                _type.dispose();
                if (_value != nullptr)
                {
                    _value->dispose();
                    delete _value;
                }
            }
        public:
            /// @brief constructor.
            inline variable_declaration_t(std::string name, std::string type, statement_t* value = nullptr)
            : declaration_t(declaration_type::variable)
            {
                _name = name;
                _type = type;
                _value = value;
            }
            /// @brief destructor.
            inline ~variable_declaration_t()
            {
            }
        public:
            /// @brief get the variable's name.
            inline std::string name() const
            {
                return _name;
            }
            /// @brief get the variable's type.
            inline std::string type() const
            {
                return _type;
            }
            /// @brief get the variable's value.
            inline statement_t* value() const
            {
                return _value;
            }
        };
        /// @brief class representing a function declaration.
        class function_declaration_t: public declaration_t
        {
        private:
            /// @brief the function's name.
            std::string _name;
            /// @brief the function's return type.
            std::string _return_type;
            /// @brief the function's arguments.
            std::arraylist<variable_declaration_t*> _arguments;
            /// @brief internal function to dispose the statement.
            void internal_dispose() override
            {
                _name.dispose();
                _return_type.dispose();
                uint32_t i;
                for(i = 0; i < _arguments.length(); i++)
                {
                    _arguments.dispose();
                    delete _arguments[i];
                }
                _arguments.dispose();
            }
        public:
            /// @brief constructor.
            inline function_declaration_t(std::string name, std::string return_type, std::arraylist<variable_declaration_t*> arguments)
            : declaration_t(declaration_type::function)
            {
                _name = name;
                _return_type = return_type;
                _arguments = arguments;
            }
            /// @brief destructor.
            inline ~function_declaration_t()
            {
            }
        public:
            /// @brief get the function's name.
            inline std::string name() const
            {
                return _name;
            }
            /// @brief get the function's return type.
            inline std::string return_type() const
            {
                return _return_type;
            }
            /// @brief get the function's arguments.
            inline std::arraylist<variable_declaration_t*> arguments() const
            {
                return _arguments;
            }
        };
        /// @brief class representing an expression.
        class expression_t: public statement_t
        {
        private:
            /// @brief expression's left operand.
            statement_t* _left;
            /// @brief expression's right operand.
            statement_t* _right;
            /// @brief internal function to dispose the statement.
            inline void internal_dispose() override
            {
                _left->dispose();
                _right->dispose();
                delete _left;
                delete _right;
            }
        public:
            /// @brief constructor.
            inline expression_t(statement_t* left, statement_t* right)
            : statement_t(statement_type::expression) 
            {
                _left = left;
                _right = right;
            }
            /// @brief destructor.
            inline ~expression_t()
            {
            }
        public:
            /// @brief get the expression's left operand.
            inline statement_t* left() const
            {
                return _left;
            }
            /// @brief get the expression's right operand.
            inline statement_t* right() const
            {
                return _right;
            }
        };
        /// @brief class representing a literal.
        class literal_t: public statement_t
        {
        public:
            enum class literal_type
            {
                null,
                integer,
                float_,
                string,
                boolean,
                character
            };
        private:
            /// @brief the literal's type.
            literal_type _type;
            /// @brief internal function to dispose the statement.
            virtual void internal_dispose() = 0;
        public:
            /// @brief constructor.
            inline literal_t(literal_type type)
            : statement_t(statement_type::literal) 
            {
                _type = type;
            }
            /// @brief destructor.
            inline ~literal_t()
            {
            }
        public:
            /// @brief get the literal's type.
            inline literal_type literal_type() const
            {
                return _type;
            }
        };
        /// @brief class representing an integer literal.
        class integer_literal_t: public literal_t
        {
        private:
            /// @brief the literal's value.
            int32_t _value;
            /// @brief internal function to dispose the statement.
            inline void internal_dispose() override
            {
                _value = 0;
            }
        public:
            /// @brief constructor.
            inline integer_literal_t(int32_t value)
            : literal_t(literal_type::integer) 
            {
                _value = value;
            }
            /// @brief destructor.
            inline ~integer_literal_t()
            {
            }
        public:
            /// @brief get the literal's value.
            inline int32_t value() const
            {
                return _value;
            }
        };
        /// @brief class representing a float literal.
        class float_literal_t: public literal_t
        {
        private:
            /// @brief the literal's value.
            float _value;
            /// @brief internal function to dispose the statement.
            inline void internal_dispose() override
            {
                _value = 0.0f;
            }
        public:
            /// @brief constructor.
            inline float_literal_t(float value)
            : literal_t(literal_type::float_) 
            {
                _value = value;
            }
            /// @brief destructor.
            inline ~float_literal_t()
            {
            }
        public:
            /// @brief get the literal's value.
            inline float value() const
            {
                return _value;
            }
        };
        /// @brief class representing a string literal.
        class string_literal_t: public literal_t
        {
        private:
            /// @brief the literal's value.
            std::string _value;
            /// @brief internal function to dispose the statement.
            inline void internal_dispose() override
            {
                _value.dispose();
            }
        public:
            /// @brief constructor.
            inline string_literal_t(std::string value)
            : literal_t(literal_type::string) 
            {
                _value = value;
            }
            /// @brief destructor.
            inline ~string_literal_t()
            {
            }
        public:
            /// @brief get the literal's value.
            inline std::string value() const
            {
                return _value;
            }
        };
        /// @brief class representing a boolean literal.
        class boolean_literal_t: public literal_t
        {
        private:
            /// @brief the literal's value.
            bool _value;
            /// @brief internal function to dispose the statement.
            inline void internal_dispose() override
            {
                _value = false;
            }
        public:
            /// @brief constructor.
            inline boolean_literal_t(bool value)
            : literal_t(literal_type::boolean) 
            {
                _value = value;
            }
            /// @brief destructor.
            inline ~boolean_literal_t()
            {
            }
        public:
            /// @brief get the literal's value.
            inline bool value() const
            {
                return _value;
            }
        };
        /// @brief class representing a character literal.
        class character_literal_t: public literal_t
        {
        private:
            /// @brief the literal's value.
            char _value;
            /// @brief internal function to dispose the statement.
            inline void internal_dispose() override
            {
                _value = '\0';
            }
        public:
            /// @brief constructor.
            inline character_literal_t(char value)
            : literal_t(literal_type::character) 
            {
                _value = value;
            }
            /// @brief destructor.
            inline ~character_literal_t()
            {
            }
        public:
            /// @brief get the literal's value.
            inline char value() const
            {
                return _value;
            }
        };
        /// @brief class representing a null literal.
        class null_literal_t: public literal_t
        {
        private:
            /// @brief internal function to dispose the statement.
            inline void internal_dispose() override
            {
            }
        public:
            /// @brief constructor.
            inline null_literal_t()
            : literal_t(literal_type::null) 
            {
            }
            /// @brief destructor.
            inline ~null_literal_t()
            {
            }
        };
        /// @brief class representing an operand.
        class operand_t: public statement_t
        {
        public:
            enum class operand_type
            {
                null,
                identifier,
                literal,
                expression
            };
        private:
            /// @brief the operand's type.
            operand_type _type;
            /// @brief statement representing the operand.
            statement_t* _statement;
            /// @brief internal function to dispose the statement.
            inline void internal_dispose() override
            {
                if (_statement != nullptr)
                {
                    _statement->dispose();
                    delete _statement;
                    _statement = nullptr;
                }
            }
        public:
            /// @brief constructor.
            inline operand_t(operand_type type, statement_t* statement)
            : statement_t(statement_type::operand) 
            {
                _type = type;
                _statement = statement;
            }
            /// @brief destructor.
            inline ~operand_t()
            {
            }
        public:
            /// @brief get the operand's type.
            inline operand_type operand_type() const
            {
                return _type;
            }
            /// @brief get the operand's statement.
            inline statement_t* statement() const
            {
                return _statement;
            }
        };
        /// @brief class representing a code block.
        class code_block_t: public statement_t
        {
        private:
            /// @brief the code block's statements.
            std::arraylist<statement_t*> _statements;
            /// @brief internal function to dispose the statement.
            inline void internal_dispose() override
            {
                for (int i = 0; i < _statements.length(); i++)
                {
                    auto statement = _statements[i];
                    statement->dispose();
                    delete statement;
                }
                _statements.dispose();
            }
        public:
            /// @brief constructor.
            inline code_block_t()
            : statement_t(statement_type::block) 
            {
            }
            /// @brief destructor.
            inline ~code_block_t()
            {
            }
        public:
            /// @brief add a statement to the code block.
            inline void add_statement(statement_t* statement)
            {
                _statements.add(statement);
            }
            /// @brief get the code block's statements.
            inline std::arraylist<statement_t*> statements() const
            {
                return _statements;
            }
        };
    }
}