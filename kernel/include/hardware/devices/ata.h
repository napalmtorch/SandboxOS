#pragma once
#include <stdint.h>
#include <stddef.h>
#include <hardware/device.h>

namespace os
{
    namespace hal
    {
        class ata_controller : public device_t
        {
            public:
                
                void init() override;
                void start() override;
                void stop() override;
        };
    }
}