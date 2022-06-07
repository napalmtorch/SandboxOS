#pragma once
#include <stdint.h>
#include <hardware/common/serial.h>

namespace os
{
    namespace sys
    {
        class debug
        {
            public:
                static hal::serial_controller serial;
                static bool                   stdout_enabled;

            public:
                static void init(hal::serial_port port);
        };
    }
}