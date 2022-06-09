#pragma once
#include <stdint.h>
#include <stddef.h>
#include <arraylist.h>
#include <hardware/device.h>
#include <hardware/pci.h>

namespace os
{
    namespace hal
    {
        /// @brief Device management services
        class device_manager
        {
            private:
                /// @brief List of detected devices
                static std::arraylist<device_t*> _devices;

            public:
                /// @brief Initialize device manager
                static void init();
                /// @brief Probe for devices
                static void probe();
                /// @brief Probe for PCI devices
                static void probe_pci();

            public:
                /// @brief Acknowledge device and add to list @param dev Pointer to device @return Device installed successfully
                static bool install(device_t* dev, bool auto_init = true);
                /// @brief Uninstall already-detected device @param dev Pointer to device @return Device uninstalled successfully
                static bool uninstall(device_t* dev);

            public:
                /// @brief Get list of detected devices @return Pointer to device list
                static std::arraylist<device_t*>* devices();
        };
    }
}