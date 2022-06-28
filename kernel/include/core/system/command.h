#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <core/exec/thread.h>
#include <core/services/terminal.h>

namespace os
{
    namespace sys
    {
        typedef struct
        {
            services::terminal_host* term;
            char*                    input;
            char**                   argv;
            int                      argc;
        } PACKED command_args_t;

        typedef void (*command_protocol_t)(command_args_t args);

        typedef struct
        {
            char*              name;
            char*              help;
            char*              usage;
            command_protocol_t protocol;
        } PACKED command_t;

        class command_handler
        {
            public:
                static threading::thread_t* thread;
                static std::arraylist<command_t*>  cmds;
                static std::arraylist<command_args_t>       queue;

            public:
                static void init();
                static void push(services::terminal_host* term, char* input);

            private:
                static int main(int argc, char** argv);
        };

        namespace command_protocols
        {
            void CLS(command_args_t args);
            void ECHO(command_args_t args);
            void TERM(command_args_t args);
            void HELP(command_args_t args);
        }

        namespace commands
        {
            static const command_t CLS  = (command_t){ .name = "CLS",  .help = "Clear the terminal",     .usage = "cls [fg] [bg]", .protocol = command_protocols::CLS  };
            static const command_t ECHO = (command_t){ .name = "ECHO", .help = "Print a string of text", .usage = "echo [str]",    .protocol = command_protocols::ECHO };
            static const command_t TERM = (command_t){ .name = "TERM", .help = "Create a new terminal instance", .usage = "term [args]", .protocol = command_protocols::TERM };
            static const command_t HELP = (command_t){ .name = "HELP", .help = "Print list of commands", .usage = "help [-u: usage][-n: name]", .protocol = command_protocols::HELP };
        }
    }
}