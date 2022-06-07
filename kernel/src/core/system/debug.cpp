#include <core/system/debug.h>
#include <core/kernel.h>

namespace os
{
    namespace sys
    {
        /// @brief Serial controller used for debugging output
        hal::serial_controller debug::serial;

        /// @brief Toggle whether printf outputs to serial
        bool debug::stdout_enabled = false;

        /// @brief Initialize debugging
        void debug::init(hal::serial_port port)
        {
            stdout_enabled = true;
            debug::serial.begin(port);
            debug::serial.print_fmt("%s Enabled serial debugging on %s\n", DEBUG_OK, debug::serial.portstr(port));
        }
    }
}