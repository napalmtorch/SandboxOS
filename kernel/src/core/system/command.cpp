#include <core/system/command.h>
#include <core/kernel.h>

namespace os
{
    namespace sys
    {
        threading::thread_t*       command_handler::thread;
        std::arraylist<command_t*> command_handler::cmds;
        std::arraylist<command_args_t>      command_handler::queue;

        void command_handler::init()
        {
            thread = threading::thread_create("cmdhost", 256 * KB, main, 0, NULL);
            cmds   = std::arraylist<command_t*>();
            queue  = std::arraylist<command_args_t>();

            cmds.add((command_t*)&commands::CLS);
            cmds.add((command_t*)&commands::ECHO);
            cmds.add((command_t*)&commands::HELP);
            cmds.add((command_t*)&commands::TERM);

            threading::scheduler::load(thread);
            threading::scheduler::start(thread);

            printf("%s Initialized command handler\n", DEBUG_OK);
        }

        void command_handler::push(services::terminal_host* term, char* input)
        {
            command_args_t args;
            args.term  = term;
            args.input = (char*)tmalloc(strlen(input) + 1, ALLOCTYPE_STRING);
            args.argc  = 0;
            args.argv  = strsplit(input, ' ', &args.argc);
            strcpy(args.input, input);
            queue.add(args);
            yield();
        }

        int command_handler::main(int argc, char** argv)
        {
            lock();
            printf("%s Entered command handler thread main\n", DEBUG_OK);
            unlock();

            while (true)
            {
                lock();
                threading::thread_monitor();

                while (true)
                {
                    if (queue.length() == 0) { break; }
                    command_args_t input = queue[0];
                    queue.remove_at(0);

                    if (input.argv == NULL || input.argc == 0)
                    {
                        free(input.input);
                        freearray((void**)input.argv, input.argc);
                        continue;
                    }

                    char* cmd = (char*)tmalloc(strlen(input.argv[0]) + 1, ALLOCTYPE_STRING);
                    strcpy(cmd, input.argv[0]);
                    strupper(cmd);

                    bool success = false;
                    for (size_t i = 0; i < cmds.length(); i++)
                    {
                        if (!strcmp(cmd, cmds[i]->name))
                        {
                            cmds[i]->protocol(input);
                            free(cmd);
                            free(input.input);
                            freearray((void**)input.argv, input.argc);
                            success = true;
                            break;
                        }
                    }

                    if (!success)
                    {
                        if (input.term != NULL) { input.term->print("Invalid command ", (uint32_t)std::color32::red); input.term->println(cmd); }
                        else { printf("%s Invalid command '%s'\n", DEBUG_ERROR, cmd); }
                        free(cmd);
                        free(input.input);
                        freearray((void**)input.argv, input.argc);
                    }

                    if (queue.length() == 0 && input.term != NULL) { input.term->print_caret(); }
                }

                unlock();
                yield();
            }
        }

        namespace command_protocols
        {
            void CLS(command_args_t args)
            {
                if (args.term != NULL) { args.term->clear(); }
            }

            void ECHO(command_args_t args)
            {
                char* input = (char*)args.input + 5;
                if (args.term != NULL) { args.term->println(input); }
                else { printf("%s\n", input); }
            }

            void TERM(command_args_t args)
            {
                kernel::shell->winmgr.load(new os::services::terminal_host(-1, -1, 80, 25, 0xFFFFFFFF, 0xFF000000, os::sys::assets::font_square));
            }

            void HELP(command_args_t args)
            {
                bool u = false, n = false;
                if (args.argc > 1)
                {
                    if (!strcmp(args.argv[1], "-u")) { u = true; n = false; }
                    if (!strcmp(args.argv[1], "-n")) { n = true; u = false; }
                    if (n && args.argc != 3) 
                    {
                        if (args.term != NULL) { args.term->println("Expected command name", (uint32_t)std::color32::red); return; }
                        else { printf("%s Expected command name\n", DEBUG_ERROR); return; }
                    }
                }

                if (!n)
                {
                    for (size_t i = 0; i < command_handler::cmds.length(); i++)
                    {
                        if (args.term != NULL)
                        {
                            args.term->print(command_handler::cmds[i]->name);
                            args.term->set_cursor_x(20);
                            if (u) { args.term->println(command_handler::cmds[i]->usage, (uint32_t)std::color32::yellow); }
                            else { args.term->println(command_handler::cmds[i]->help, (uint32_t)std::color32::lime); }
                        }
                        else
                        {
                            char name[64];
                            memset(name, 0, sizeof(name));
                            strcpy(name, command_handler::cmds[i]->name);
                            while (strlen(name) < 20) { stradd(name, ' '); }
                            if (u) { printf("%s %s\n", name, command_handler::cmds[i]->usage); }
                            else { printf("%s %s\n", name, command_handler::cmds[i]->help); }
                        }
                    }
                }
                else
                {
                    for (size_t i = 0; i < command_handler::cmds.length(); i++)
                    {
                        if (!strcmp(command_handler::cmds[i]->name, strupper(args.argv[2])))
                        {
                            if (args.term != NULL)
                            {
                                args.term->print("NAME             ", (uint32_t)std::color32::yellow);
                                args.term->println(command_handler::cmds[i]->name);
                                args.term->print("DESCRIPTION      ", (uint32_t)std::color32::yellow);
                                args.term->println(command_handler::cmds[i]->help);
                                args.term->print("USAGE            ", (uint32_t)std::color32::yellow);
                                args.term->println(command_handler::cmds[i]->usage);
                            }
                            else
                            {
                                printf("NAME             %s\n", command_handler::cmds[i]->name);
                                printf("DESCRIPTION      %s\n", command_handler::cmds[i]->help);
                                printf("USAGE            %s\n", command_handler::cmds[i]->usage);
                            }
                            return;
                        }
                    }

                    if (args.term != NULL) { args.term->println("No command exists with specified name", (uint32_t)std::color32::red); }
                    else { printf("%s No command exists with specified name\n", DEBUG_ERROR); }
                }
            }
        }
    }
}