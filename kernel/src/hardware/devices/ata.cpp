#include <hardware/devices/ata.h>
#include <core/kernel.h>

namespace os
{
    namespace hal
    {
        namespace devices
        {
            ata_controller* ata;
        }

        void ata_controller::init()
        {
            device_t::init();
            set_info("Programmed I/O ATA Controller", 0xD0000000);
        }

        void ata_controller::start()
        {
            device_t::start();
        }

        void ata_controller::stop()
        {
            device_t::stop();
        }
    }
}