#include <hardware/devmgr.h>
#include <core/kernel.h>

namespace os
{
    namespace hal
    {
        std::arraylist<device_t*> device_manager::_devices;

        void device_manager::init()
        {
            // create device list
            _devices = std::arraylist<device_t*>();

            // probe for devices
            probe_pci();
            probe();

            // start all installed devices
            for (size_t i = 0; i < _devices.length(); i++) { _devices[i]->start(); }

            // successfully finished
            printf("%s Initialized device manager - %u devices detected\n", DEBUG_INFO, _devices.length());
        }

        void device_manager::probe()
        {
            printf("%s Probing on-board devices...\n", DEBUG_INFO);

            // initialize pit
            hal::pit::init();

            // initialize rtc
            hal::rtc::init();

            // install serial
            devices::serial = new hal::serial_controller();
            install(devices::serial);
            devices::serial->start(sys::debug::serial->port());
            sys::debug::serial = devices::serial;

            // install ata
            devices::ata = new hal::ata_controller();
            install(devices::ata);

            // install keyboard
            devices::kbd = new hal::ps2_keyboard();
            install(devices::kbd);
            
            // install mouse
            devices::mouse = new hal::ps2_mouse();
            install(devices::mouse);

            // initialized ps/2 controller
            ps2_controller::init();

            // install vbe
            pcihdr_t* vbe_device = pci_controller::fetch(0x1234, 0x1111);
            if (vbe_device == NULL) { printf("%s Failed to locate VBE compatible device\n", DEBUG_ERROR); }
            else
            {
                devices::vbe = new hal::vbe_controller();
                devices::vbe->pcidev = vbe_device;
                devices::vbe->pcidev->device_drv = devices::vbe;
                install(devices::vbe);
            }
        }

        void device_manager::probe_pci()
        {
            printf("%s Probing PCI devices...\n", DEBUG_INFO);
            pci_controller::probe();
        }

        bool device_manager::install(device_t* dev, bool auto_init)
        {
            if (dev == NULL) { perror("Tried to install null device"); return false; }
            _devices.add(dev);
            if (auto_init) { dev->init(); }
            printf("%s Installed device - ADDR:0x%8x ID:0x%8x NAME:'%s'\n", DEBUG_INFO, dev, dev->id(), dev->name());
            return true;
        }

        bool device_manager::uninstall(device_t* dev)
        {
            if (dev == NULL) { return false; }
            for (size_t i = 0; i < _devices.length(); i++)
            {
                if (_devices[i] == dev)
                {
                    _devices.remove_at(i);
                    printf("%s Uninstalled device - ADDR:0x%8x ID:%u NAME:'%s'\n", DEBUG_INFO, dev, dev->id(), dev->name());
                    return true;
                }
            }
            perror("Failed to uninstall device at 0x%8x", dev);
            return false;
        }

        std::arraylist<device_t*>* device_manager::devices() { return &_devices; }
    }
}