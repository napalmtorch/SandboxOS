#include <hardware/pci.h>
#include <core/kernel.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC

namespace os
{
    namespace hal
    {
        std::arraylist<pcihdr_t*> pci_controller::devices;

        void pci_controller::probe()
        {
            devices = std::arraylist<pcihdr_t*>();
            for (uint16_t bus = 0; bus < 256; bus++)
            {
                for (uint16_t slot = 0; slot < 32; slot++)
                {
                    for (uint16_t func = 0; func < 8; func++)
                    {
                        uint16_t vend_id  = read_word(bus, slot, func, 0x00);
                        if (vend_id == 0xFFFF) { continue; }

                        uint8_t hdr_type = (uint8_t)(read_word(bus, slot, func, 0x0E) & 0xFF);
                        hdr_type &= ~(1 << 7);
                        if (hdr_type == (uint8_t)pcihdr_type::DEVICE)
                        {
                            pcihdr_device_t* dev = new pcihdr_device_t();
                            pcihdr_t devhdr = probe_device(bus, slot, func);
                            memcpy(dev, &devhdr, sizeof(pcihdr_t));
                            devices.add(dev);
                            printf("%s Detected device - VEN_ID:0x%4x DEV_ID:0x%4x NAME:'%s'\n", DEBUG_INFO, dev->vendor_id, dev->device_id, devname(dev->vendor_id, dev->device_id));
                        }
                        else if (hdr_type == (uint8_t)pcihdr_type::BRIDGE)
                        {
                            pcihdr_bridge_t* dev = new pcihdr_bridge_t();
                            pcihdr_t devhdr = probe_device(bus, slot, func);
                            memcpy(dev, &devhdr, sizeof(pcihdr_t));
                            devices.add(dev);
                            printf("%s Detected PCI-to-PCI bridge - VEN_ID:0x%4x DEV_ID:0x%4x NAME:'%s'\n", DEBUG_INFO, dev->vendor_id, dev->device_id, devname(dev->vendor_id, dev->device_id));
                        }
                        else if (hdr_type == (uint8_t)pcihdr_type::CARDBUS)
                        {
                            pcihdr_carbus_t* dev = new pcihdr_carbus_t();
                            pcihdr_t devhdr = probe_device(bus, slot, func);
                            memcpy(dev, &devhdr, sizeof(pcihdr_t));
                            devices.add(dev);
                            printf("%s Detected PCI-to-Cardbus bridge - VEN_ID:0x%4x DEV_ID:0x%4x NAME:'%s'\n", DEBUG_INFO, dev->vendor_id, dev->device_id, devname(dev->vendor_id, dev->device_id));
                        }
                        else { perror("Invalid PCI header type 0x%2x", hdr_type); }
                    }
                }
            }
        }

        pcihdr_t pci_controller::probe_device(uint16_t bus, uint16_t slot, uint16_t func)
        {
            pcihdr_t hdr;
            hdr.vendor_id     = read_word(bus, slot, func, 0x00);
            hdr.device_id     = read_word(bus, slot, func, 0x02);
            hdr.command       = read_word(bus, slot, func, 0x04);
            hdr.status        = read_word(bus, slot, func, 0x06);
            hdr.rev_id        = (uint8_t)(read_word(bus, slot, func, 0x08) & 0xFF);
            hdr.prog_if       = (uint8_t)(read_word(bus, slot, func, 0x08) >> 8);
            hdr.subclass      = (uint8_t)(read_word(bus, slot, func, 0x0A) & 0xFF);
            hdr.class_code    = (uint8_t)(read_word(bus, slot, func, 0x0A) >> 8);
            hdr.cacheln_sz    = (uint8_t)(read_word(bus, slot, func, 0x0C) & 0xFF);
            hdr.latency_timer = (uint8_t)(read_word(bus, slot, func, 0x0C) >> 8);
            hdr.hdr_type      = (pcihdr_type)(read_word(bus, slot, func, 0x0E) & 0xFF);
            hdr.bist          = (uint8_t)(read_word(bus, slot, func, 0x0E) >> 8);
            return hdr;
        }

        void pci_controller::write_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset, uint16_t data)
        {   
            uint32_t addr = (uint32_t)((bus << 16) | (slot << 1) | (func << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
            ports::outl(PCI_CONFIG_ADDRESS, addr);
            ports::outl(PCI_CONFIG_DATA, data);
        }

        uint16_t pci_controller::read_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset)
        {
            uint32_t addr  = 0;
            uint32_t lbus  = bus;
            uint32_t lslot = slot;
            uint32_t lfunc = func;
            uint16_t temp  = 0;
            addr = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
            ports::outl(PCI_CONFIG_ADDRESS, addr);
            temp = (uint16_t)((ports::inl(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
            return (temp);
        }

        pcihdr_t* pci_controller::fetch(uint16_t vendor_id, uint16_t device_id)
        {
            if (vendor_id == 0xFFFF) { return NULL; }        
            for (size_t i = 0; i < devices.length(); i++)
            {
                if (devices[i]->vendor_id == vendor_id && devices[i]->device_id == device_id) { return devices[i]; }
            }
            return NULL;
        }
    
        const char* pci_controller::devname(uint16_t vendor_id, uint16_t device_id)
        {
            pci_vendor vendor = (pci_vendor)vendor_id;

            // intel devices
            if (vendor == pci_vendor::INTEL)
            {
                switch (device_id)
                {
                    case 0x1237: { return "Intel 440FX Chipset"; }
                    case 0x7000: { return "Intel 82371SB PIIX3 ISA"; }
                    case 0x7010: { return "Intel 82371SB PIIX3 IDE"; }
                    case 0x7020: { return "Intel 82371SB PIIX3 USB"; }
                    case 0x7111: { return "Intel 82371AB/EB/MB PIIX4 IDE"; }
                    case 0x7113: { return "Intel 82371AB/EB/MB PIIX4 ACPI"; }
                    case 0x100E: { return "Intel 82540EM Ethernet Controller"; }
                    case 0x0041: { return "Intel Core PCI Express x16 Root Port"; }
                    case 0x0042: { return "Intel Core IGPU Controller"; }
                    case 0x0044: { return "Intel Core DRAM Controller"; }
                    case 0x0600: { return "Intel RAID Controller"; }
                    case 0x061F: { return "Intel 80303 I/O Processor"; }
                    case 0x2415: { return "Intel 82801AA AC'97 Audio"; }
                    case 0x2668: { return "Intel 82801(ICH6 Family) HD Audio"; }
                    case 0x2770: { return "Intel 82945G Memory Controller Hub"; }
                    case 0x2771: { return "Intel 82945G PCI Express Root Port"; }
                    case 0x2772: { return "Intel 82945G Integrated Graphics"; }
                    case 0x2776: { return "Intel 82945G Integrated Graphics"; }
                    case 0x27D0: { return "Intel NM10/ICH7 PCI Express Port 1"; }
                    case 0x27D2: { return "Intel NM10/ICH7 PCI Express Port 2"; }
                    case 0x27C8: { return "Intel NM10/ICH7 USB UHCI Controller 1"; }
                    case 0x27C9: { return "Intel NM10/ICH7 USB UHCI Controller 2"; }
                    case 0x27CA: { return "Intel NM10/ICH7 USB UHCI Controller 3"; }
                    case 0x27CB: { return "Intel NM10/ICH7 USB UHCI Controller 4"; }
                    case 0x27CC: { return "Intel NM10/ICH7 USB2 EHCI Controller"; }
                    case 0x244E: { return "Intel 82801 PCI Bridge"; }
                    case 0x27DE: { return "Intel 82801G/ICH7 AC'97 Audio"; }
                    case 0x27B8: { return "Intel 82801GB/ICH7 LPC Interface Bridge"; }
                    case 0x27C0: { return "Intel NM10/ICH7 SATA Controller(IDE MODE)"; }
                    case 0x27DA: { return "Intel NM10/ICH7 SMBus Controller"; }
                    case 0x0F00: { return "Intel Atom Z36xxx/Z37xxx SoC Trans-Reg"; }
                    case 0x0F31: { return "Intel Atom Z36xxx/Z37xxx Series Graphics"; }
                    case 0x0F23: { return "Intel Atom E3800 Series SATA AHCI"; }
                    case 0x0F18: { return "Intel Atom Z36xxx/Z37xxx Trusted Exec."; }
                    case 0x0F04: { return "Intel Atom Z36xxx/Z37xxx HD Audio"; }
                    case 0x0F48: { return "Intel Atom E3800 Series PCI Express Port 1"; }
                    case 0x0F4A: { return "Intel Atom E3800 Series PCI Express Port 2"; }
                    case 0x0F34: { return "Intel Atom Z36xxx/Z37xxx USB EHCI"; }
                    case 0x0F1C: { return "Intel Atom Z36xxx/Z37xxx Series PCU"; }
                    case 0x0F12: { return "Intel Atom E3800 Series SMBus Controller"; }
                    default:     { return "Unrecognized Intel Device"; }
                }
            }

            // newer intel devices
            if (vendor == pci_vendor::INTEL_ALT)
            {
                switch (device_id)
                {
                    default: { return "Unrecognized Intel Device"; }
                }
            }

            // vmware
            if (vendor == pci_vendor::VMWARE)
            {
                switch (device_id)
                {
                    case 0x0405: { return "VMWare SVGAII Adapter"; }
                    case 0x0710: { return "VMWare SVGA Adapter"; }
                    case 0x0770: { return "VMWare USB2 EHCI Controller"; }
                    case 0x0720: { return "VMWare VMXNET Ethernet Controller"; }
                    case 0x0740: { return "VMWare VM Communication Interface"; }
                    case 0x0774: { return "VMWare USB1.1 UHCI Controller"; }
                    case 0x0778: { return "VMWare USB3 xHCI 0.96 Controller"; }
                    case 0x0779: { return "VMWare USB3 xHCI 1.00 Controller"; }
                    case 0x0790: { return "VMWare PCI Bridge"; }
                    case 0x07A0: { return "VMWare PCI Express Root Port"; }
                    case 0x07B0: { return "VMWare VMXNET3 Ethernet Controller"; }
                    case 0x07C0: { return "VMWare PVSCSI SCSI Controller"; }
                    case 0x07E0: { return "VMWare SATA AHCI Controller"; }
                    case 0x0801: { return "VMWare VM Interface"; }
                    case 0x0820: { return "VMWare Paravirtual RDMA Controller"; }
                    case 0x0800: { return "VMWare Hypervisor ROM Interface"; }
                    case 0x1977: { return "VMWare HD Audio Controller"; }
                    default: { return "Unrecognized VMWare Device"; }
                }
            }

            // vmware
            if (vendor == pci_vendor::VMWARE_ALT)
            {
                switch (device_id)
                {
                    default: { return "Unrecognized VMWare Device"; }
                }
            }

            // innotek
            if (vendor == pci_vendor::INNOTEK)
            {
                switch (device_id)
                {
                    case 0xBEEF: { return "VirtualBox Graphics Adapter"; }
                    case 0xCAFE: { return "VirtualBox Guest Service"; }
                    default: { return "Unrecognized InnoTek Device"; }
                }
            }

            // amd
            if (vendor == pci_vendor::AMD)
            {
                switch (device_id)
                {
                    case 0x2000: { return "AMD 79C970 Ethernet Controller"; }
                    default: { return "Unrecognized AMD Device"; }
                }
            }

            // ensoniq
            if (vendor == pci_vendor::ENSONIQ)
            {
                switch (device_id)
                {
                    case 0x5000: { return "Ensoniq ES1370 [AudioPCI]"; }
                    default: { return "Unrecognized Ensoniq Device"; }
                }
            }

            // broadcom
            if (vendor == pci_vendor::BROADCOM)
            {
                switch (device_id)
                {
                    case 0x1677: { return "NetXtreme BCM5751 Ethernet PCI-E"; }
                    default: { return "Unrecognized Realtek Device"; }
                }
            }

            // realtek
            if (vendor == pci_vendor::REALTEK)
            {
                switch (device_id)
                {
                    case 0x5289: { return "Realtek RTL8411 PCI Express Card Reader"; }
                    case 0x8168: { return "Realtek RTL8111/8168/8411 Ethernet PCI-E"; }
                    default: { return "Unrecognized Realtek Device"; }
                }
            }

            // atheros
            if (vendor == pci_vendor::ATHEROS)
            {
                switch (device_id)
                {
                    case 0x0036: { return "Atheros AR9485 WiFi Adapter"; }
                    default: { return "Unrecognized Atheros Device"; }
                }
            }

            // qemu graphics controller
            if (vendor_id == 0x1234 && device_id == 0x1111) { return "QEMU VGA Controller"; }

            // unknown
            return "Unrecognized Device";
        }
    }
}