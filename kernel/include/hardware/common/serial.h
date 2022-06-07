#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

namespace os
{
    namespace hal
    {
        enum class serial_port : uint16_t
        {
            disabled = 0x000,
            com1     = 0x3F8,
            com2     = 0x2F8,
            com3     = 0x3E8,
            com4     = 0x2E8,
        };

        static const char* serial_portnames[5] = 
        {
            "disabled",
            "com1",
            "com2",
            "com3",
            "com4",
        };

        class serial_controller
        {
            private:
                serial_port _port;

            public:
                void begin(serial_port port);
                void end();

            public:
                char read();
                void write(char c);
                void print(char* str);
                void println(char* str);
                void vprint_fmt(char* fmt, va_list args);
                void print_fmt(char* fmt, ...);

            private:
                bool can_send();
                bool has_recv();
                bool validate();

            public:
                static const char* portstr(serial_port port);
                serial_port port();
        };
    }
}