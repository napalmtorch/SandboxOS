#include <hardware/devices/ata.h>
#include <core/kernel.h>

// ports
#define ATA_PRIMARY_DATA         0x1F0
#define ATA_PRIMARY_ERR          0x1F1
#define ATA_PRIMARY_SECCOUNT     0x1F2
#define ATA_PRIMARY_LBA_LO       0x1F3
#define ATA_PRIMARY_LBA_MID      0x1F4
#define ATA_PRIMARY_LBA_HI       0x1F5
#define ATA_PRIMARY_DRIVE_HEAD   0x1F6
#define ATA_PRIMARY_COMM_REGSTAT 0x1F7
#define ATA_PRIMARY_ALTSTAT_DCR  0x3F6

// status flags
#define ATA_STAT_ERR  (1 << 0) 
#define ATA_STAT_DRQ  (1 << 3)
#define ATA_STAT_SRV  (1 << 4)
#define ATA_STAT_DF   (1 << 5) 
#define ATA_STAT_RDY  (1 << 6)
#define ATA_STAT_BSY  (1 << 7)

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
            _identified = false;
            set_info("Programmed I/O ATA Controller", 0xD0000000);
        }

        void ata_controller::start()
        {
            device_t::start();
            idt::register_irq(IRQ::IRQ14, callback);
            _identified = identify();

            if (!_identified) { printf("%s Primary ATA disk not detected\n", DEBUG_WARN); stop(); return; }
            printf("%s Located primary ATA disk\n", DEBUG_OK);
        }

        void ata_controller::stop()
        {
            device_t::stop();
            _identified = false;
            idt::unregister_irq(IRQ::IRQ14);
        }

        void ata_controller::callback(idt_registers_t* regs)
        {
            
        }
        
        bool ata_controller::identify()
        {
            if (!_running) { return false; }
            ports::inb(ATA_PRIMARY_COMM_REGSTAT);
            ports::outb(ATA_PRIMARY_DRIVE_HEAD, 0xA0);
            ports::inb(ATA_PRIMARY_COMM_REGSTAT);
            ports::outb(ATA_PRIMARY_SECCOUNT, 0);
            ports::inb(ATA_PRIMARY_COMM_REGSTAT);
            ports::outb(ATA_PRIMARY_LBA_LO, 0);
            ports::inb(ATA_PRIMARY_COMM_REGSTAT);
            ports::outb(ATA_PRIMARY_LBA_MID, 0);
            ports::inb(ATA_PRIMARY_COMM_REGSTAT);
            ports::outb(ATA_PRIMARY_LBA_HI, 0);
            ports::inb(ATA_PRIMARY_COMM_REGSTAT);
            ports::outb(ATA_PRIMARY_COMM_REGSTAT, 0xEC);
            ports::outb(ATA_PRIMARY_COMM_REGSTAT, 0xE7);

            // Read the status port. If it's zero, the drive does not exist.
            uint8_t status = ports::inb(ATA_PRIMARY_COMM_REGSTAT);

            while (status & ATA_STAT_BSY) 
            {
                uint32_t i = 0;
                while(1) { i++; }
                for(i = 0; i < 0x0FFFFFFF; i++) { }
                status = ports::inb(ATA_PRIMARY_COMM_REGSTAT);
            }
            
            if (status == 0) { return false; }

            while (status & ATA_STAT_BSY) { status = ports::inb(ATA_PRIMARY_COMM_REGSTAT); }

            uint8_t mid = ports::inb(ATA_PRIMARY_LBA_MID);
            uint8_t hi = ports::inb(ATA_PRIMARY_LBA_HI);
            if (mid || hi) { return false; }

            // Wait for ERR or DRQ
            while (!(status & (ATA_STAT_ERR | ATA_STAT_DRQ))) { status = ports::inb(ATA_PRIMARY_COMM_REGSTAT); }

            if (status & ATA_STAT_ERR) { return false; }

            uint8_t* buff = (uint8_t*)tmalloc(512, ALLOCTYPE_ARRAY);
            ports::insw(ATA_PRIMARY_DATA, buff, 256);
            free(buff);
            return true;
        }
        
        bool ata_controller::read(uint64_t sector, uint32_t count, uint8_t* buff)
        {
            if (!_running) { return false; }
            ports::outb(ATA_PRIMARY_DRIVE_HEAD, 0x40);                        // Select master
            ports::outb(ATA_PRIMARY_SECCOUNT, (count >> 8) & 0xFF );          // sectorcount high
            ports::outb(ATA_PRIMARY_LBA_LO, (sector >> 24) & 0xFF);           // LBA4
            ports::outb(ATA_PRIMARY_LBA_MID, (sector >> 32) & 0xFF);          // LBA5
            ports::outb(ATA_PRIMARY_LBA_HI, (sector >> 40) & 0xFF);           // LBA6
            ports::outb(ATA_PRIMARY_SECCOUNT, count & 0xFF);                  // sectorcount low
            ports::outb(ATA_PRIMARY_LBA_LO, sector & 0xFF);                   // LBA1
            ports::outb(ATA_PRIMARY_LBA_MID, (sector >> 8) & 0xFF);           // LBA2
            ports::outb(ATA_PRIMARY_LBA_HI, (sector >> 16) & 0xFF);           // LBA3
            ports::outb(ATA_PRIMARY_COMM_REGSTAT, 0x24);                      // READ SECTORS EXT

            for (uint32_t i = 0; i < count; i++) 
            {
                while (true)
                {
                    uint8_t status = ports::inb(ATA_PRIMARY_COMM_REGSTAT);
                    if (status & ATA_STAT_DRQ) { break; }
                    if (status & ATA_STAT_ERR) { perror("Disk read error"); return false; }
                }
                ports::insw(ATA_PRIMARY_DATA, buff, 256);
                buff += 256;
            }
            return true;
        }
        
        bool ata_controller::write(uint64_t sector, uint32_t count, uint8_t* buff)
        {
            if (!_running) { return false; }
            ports::outb(ATA_PRIMARY_DRIVE_HEAD, 0x40);                     // Select master
            ports::outb(ATA_PRIMARY_SECCOUNT, (count >> 8) & 0xFF );     // sectorcount high
            ports::outb(ATA_PRIMARY_LBA_LO, (sector >> 24) & 0xFF);           // LBA4
            ports::outb(ATA_PRIMARY_LBA_MID, (sector >> 32) & 0xFF);          // LBA5
            ports::outb(ATA_PRIMARY_LBA_HI, (sector >> 40) & 0xFF);           // LBA6
            ports::outb(ATA_PRIMARY_SECCOUNT, count & 0xFF);             // sectorcount low
            ports::outb(ATA_PRIMARY_LBA_LO, sector & 0xFF);                   // LBA1
            ports::outb(ATA_PRIMARY_LBA_MID, (sector >> 8) & 0xFF);           // LBA2
            ports::outb(ATA_PRIMARY_LBA_HI, (sector >> 16) & 0xFF);           // LBA3
            ports::outb(ATA_PRIMARY_COMM_REGSTAT, 0x34);                   // READ SECTORS EXT

            for (uint8_t i = 0; i < count; i++) 
            {
                while (true) 
                {
                    uint8_t status = ports::inb(ATA_PRIMARY_COMM_REGSTAT);
                    if (status & ATA_STAT_DRQ) { break; }
                    else if (status & ATA_STAT_ERR) { perror("Disk write error"); return false; }
                }
                ports::outsw(ATA_PRIMARY_DATA, buff, 256);
                buff += 256;
            }

            // Flush the cache.
            ports::outb(ATA_PRIMARY_COMM_REGSTAT, 0xE7);
            // Poll for BSY.
            while (ports::inb(ATA_PRIMARY_COMM_REGSTAT) & ATA_STAT_BSY);
            return true;
        }
    }
}