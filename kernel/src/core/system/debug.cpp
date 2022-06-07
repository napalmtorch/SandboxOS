#include <core/system/debug.h>
#include <core/kernel.h>

namespace os
{
    namespace sys
    {
        hal::serial_controller debug::serial;
        bool                   debug::stdout_enabled = false;

        void debug::init(hal::serial_port port)
        {
            stdout_enabled = true;
            debug::serial.begin(port);
            debug::serial.print_fmt("%s Enabled serial debugging on %s\n", DEBUG_OK, debug::serial.portstr(port));
        }
    }
}