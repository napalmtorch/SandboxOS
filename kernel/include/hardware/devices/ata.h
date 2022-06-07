#pragma once
#include <stdint.h>
#include <stddef.h>
#include <hardware/device.h>

namespace os
{
    namespace hal
    {
        /// @brief Programmed I/O ATA controller driver
        class ata_controller : public device_t
        {
            public:
                /// @brief Initialize ATA controller           
                void init() override;
                /// @brief Start ATA controller driver
                void start() override;
                /// @brief Stop ATA controller driver
                void stop() override;
        };
    }
}