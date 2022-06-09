#pragma once
#include <stdint.h>
#include <stddef.h>
#include <hardware/device.h>
#include <hardware/interrupts/idt.h>

namespace os
{
    namespace hal
    {
        /// @brief Programmed I/O ATA controller driver
        class ata_controller : public device_t
        {
            private:
                bool _identified;

            public:
                /// @brief Initialize ATA controller           
                void init() override;
                /// @brief Start ATA controller driver
                void start() override;
                /// @brief Stop ATA controller driver
                void stop() override;

            public:
                /// @brief Interrupt callback handler for ATA
                static void callback(idt_registers_t* regs);
                /// @brief Probe ATA devices
                bool identify();
                /// @brief Read data from specified sectors @param sector Starting sector @param count Amount of sectors to read @param buff Destination buffer for incoming data @return Data read successfully
                bool read(uint64_t sector, uint32_t count, uint8_t* buff);
                /// @brief Write data to specified sectors @param sector Starting sector @param count Amount of sectors to write @param buff Source buffer for outgoing data @return Data written successfully
                bool write(uint64_t sector, uint32_t count, uint8_t* buff);
        };

        namespace devices
        {
            /// @brief ATA controller device instance
            extern ata_controller* ata;
        }
    }
}