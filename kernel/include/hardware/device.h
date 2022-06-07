#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

namespace os
{
    namespace hal
    {
        /// @brief Hardware device abstraction interface
        class device_t
        {
            protected:
                /// @brief Name of devices - maximum of 128 characters
                char _name[128];
                /// @brief Unique identification number
                uint32_t _id;
                /// @brief Is device driver running
                bool _running;

            public:
                virtual void init();
                virtual void start();
                virtual void stop();

            protected:
                void set_info(char* name, uint32_t id);

            public:
                /// @brief Get unique identifier of device
                uint32_t id();
                /// @brief Get name of device
                char* name();
                /// @brief Check if device driver is running
                bool running();
        };
    }
}