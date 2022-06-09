#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <arraylist.h>

namespace os
{
    namespace hal
    {
        enum class pci_vendor : uint16_t
        {
            INTEL                = 0x8086,
            INTEL_ALT            = 0x8087,
            VMWARE               = 0x15AD,
            VMWARE_ALT           = 0x0E0F,
            DELL                 = 0x1028,
            DELL_ALT             = 0x413C,
            ATI                  = 0x1002,
            AMD                  = 0x1022,
            VIA                  = 0x2109,
            BENQ                 = 0x04A5,
            ACER                 = 0x5986,
            HP                   = 0x03F0,
            HP_ALT               = 0x103C,
            HP_ENT               = 0x1590,
            ASUS                 = 0x1043,
            ASUS_ALT             = 0x0B05,
            INNOTEK              = 0x80EE,
            REALTEK              = 0x10EC,
            REAKLTEK_ALT         = 0x0BDA,
            ENSONIQ              = 0x1274,
            BROADCOM             = 0x14E4,
            ATHEROS              = 0x168C,
        };

        enum class pcihdr_type : uint8_t
        {
            DEVICE,
            BRIDGE,
            CARDBUS,
        };

        enum class pcidev_type : uint8_t
        {
            UNCLASSIFIED        = 0x00,
            STORAGE             = 0x01,
            NETWORK             = 0x02,
            DISPLAY             = 0x03,
            MULTIMEDIA          = 0x04,
            MEMORY              = 0x05,
            BRIDGE              = 0x06,
            SIMPLE_COMM         = 0x07,
            SYS_PERIPHERAL      = 0x08,
            INPUT               = 0x09,
            DOCKING_STATION     = 0x0A,
            PROCESSOR           = 0x0B,
            SERIAL_BUS          = 0x0C,
            WIRELESS            = 0x0D,
            INTELLIGENT         = 0x0E,
            SATELLITE_COMM      = 0x0F,
            ENCRYPTION          = 0x10,
            SIGNAL_PROCESSOR    = 0x11,
            PROCESSING_ACCEL    = 0x12,
            NON_ESSENTIAL_INSTR = 0x13,
            CO_PROCESSOR        = 0x40,
            UNASSIGNED          = 0xFF,
        };

        class device_t;

        class pcihdr_t
        {
            public:
                device_t* device_drv;

            public:
                uint16_t    vendor_id;
                uint16_t    device_id;
                uint16_t    status;
                uint16_t    command;
                uint8_t     class_code;
                uint8_t     subclass;
                uint8_t     prog_if;
                uint8_t     rev_id;
                uint8_t     bist;
                pcihdr_type hdr_type;
                uint8_t     latency_timer;
                uint8_t     cacheln_sz;
        };

        class pcihdr_device_t : public pcihdr_t
        {
            public:
                uint32_t    bar0;
                uint32_t    bar1;
                uint32_t    bar2;
                uint32_t    bar3;
                uint32_t    bar4;
                uint32_t    bar5;
                uint32_t    p_cardbus_cis;
                uint16_t    subsys_id;
                uint16_t    subsys_vendor_id;
                uint32_t    exprom_bar;
                uint8_t     reserved0[3];
                uint8_t     p_capabilities;
                uint8_t     reserved1[4];
                uint8_t     max_latency;
                uint8_t     min_grant;
                uint8_t     irq_pin;
                uint8_t     irq_line;

        };

        class pcihdr_bridge_t : public pcihdr_t
        {
            public:
                uint32_t    bar0;
                uint32_t    bar1;
                uint8_t     secondary_latency_timer;
                uint8_t     subcoord_busnum;
                uint8_t     secondary_busnum;
                uint8_t     primary_busnum;
                uint16_t    secondary_status;
                uint8_t     io_limit;
                uint8_t     io_base;
                uint16_t    mem_limit;
                uint16_t    mem_base;
                uint16_t    prefetch_mem_limit;
                uint16_t    prefetch_mem_base;
                uint32_t    prefetch_base_upper;
                uint32_t    prefetch_limit_upper;
                uint16_t    io_limit_upper;
                uint16_t    io_base_upper;
                uint8_t     reserved[3];
                uint8_t     p_capabilities;
                uint32_t    exprom_bar;
                uint16_t    bridge_ctrl;
                uint8_t     irq_pin;
                uint8_t     irq_line;
        };

        class pcihdr_carbus_t : public pcihdr_t
        {
            public:
                uint32_t    cardbus_socket_bar;
                uint16_t    secondary_status;
                uint8_t     reserved;
                uint8_t     p_list_caps;
                uint8_t     cardbus_latency_timer;
                uint8_t     subcoord_busnum;
                uint8_t     cardbus_busnum;
                uint8_t     pcibus_num;
                uint32_t    mem_base0;
                uint32_t    mem_limit0;
                uint32_t    mem_base1;
                uint32_t    mem_limit1;
                uint32_t    io_base0;
                uint32_t    io_limit0;
                uint32_t    io_base1;
                uint32_t    io_limit1;
                uint16_t    bridge_ctrl;
                uint8_t     irq_pin;
                uint8_t     irq_line;
                uint16_t    subsys_vendor_id;
                uint16_t    subsys_device_id;
                uint32_t    legacy16_base;
        };

        class pci_controller
        {
            public:
                static std::arraylist<pcihdr_t*> devices;

            public:
                static void probe();
                static pcihdr_t probe_device(uint16_t bus, uint16_t slot, uint16_t func);

            public:
                static void write_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset, uint16_t data);
                static uint16_t read_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset);

                /// @brief Get PCI device from vendor and device ID - returns null if device is not detected @param vendor_id Vendor ID @param device_id Device ID @return Pointer to PCI device
                static pcihdr_t* fetch(uint16_t vendor_id, uint16_t device_id);

            public:
                static const char* devname(uint16_t vendor_id, uint16_t device_id);
        };
    }
}