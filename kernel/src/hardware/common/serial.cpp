#include <hardware/common/serial.h>
#include <core/kernel.h>

namespace os
{
    namespace hal
    {
        namespace devices
        {
            serial_controller* serial;
        }

        void serial_controller::init()
        {
            device_t::init();
            set_info("Serial I/O Controller", 0xD000FFFF);
        }

        void serial_controller::start()
        {
            device_t::start();
        }

        void serial_controller::start(serial_port port)
        {
            device_t::start();

            // set port variable
            this->_port = port;

            // send port data to serial controller
            ports::outb((portnum_t)this->_port + 1, 0x00);    // disable all interrupts
            ports::outb((portnum_t)this->_port + 3, 0x80);    // set baud rate divisor
            ports::outb((portnum_t)this->_port + 0, 0x03);    // set divisor to 3 (lo uint8_t) 38400 baud
            ports::outb((portnum_t)this->_port + 1, 0x00);    // hi uint8_t
            ports::outb((portnum_t)this->_port + 3, 0x03);    // 8 bits, no parity, one stop bit
            ports::outb((portnum_t)this->_port + 2, 0xC7);    // enable fifo, clear them, with 14-uint8_t threshold
            ports::outb((portnum_t)this->_port + 4, 0x0B);    // irqs enabled, rts/dsr set
            ports::outb((portnum_t)this->_port + 4, 0x1E);    // set in loopback mode, test the serial chip
            ports::outb((portnum_t)this->_port + 0, 0xAE);    // test serial chip (send uint8_t 0xAE and check if serial returns same uint8_t)

            // check if serial is faulty
            if (ports::inb((portnum_t)this->_port) != 0xAE) { this->_port = serial_port::disabled; return; }
            
            // ff serial is not faulty set it in normal operation mode
            ports::outb((portnum_t)this->_port + 4, 0x0F);
        }

        void serial_controller::stop()
        {
            device_t::stop();
            this->_port = serial_port::disabled;
        }

        char serial_controller::read()
        {
            if (!validate()) { return 0; }
            while (!has_recv());
            return ports::inb((portnum_t)this->_port);
        }

        void serial_controller::write(char c)
        {
            if (!validate()) { return; }
            while (!can_send());
            ports::outb((portnum_t)this->_port, c);
        }

        void serial_controller::print(char* str)
        {
            if (str == NULL) { return; }
            if (!validate()) { return; }

            int i = 0;
            while (str[i] != 0) { write(str[i]); i++; }
        }

        void serial_controller::println(char* str)
        {
            print(str);
            write('\n');
        }

        void serial_controller::vprint_fmt(char* fmt, va_list args)
        {
            while (*fmt != 0)
            {
                if (*fmt == '%')
                {
                    fmt++;
                    if (*fmt == '%') { write('%'); fmt++; continue; }
                    if (*fmt == 'c') { write((char)va_arg(args, int)); }
                    else if (*fmt == 'd')
                    {
                        int dec = va_arg(args, int);
                        char str[16];
                        memset(str, 0, 16);
                        print(itoa(dec, str, 10));
                    }
                    else if (*fmt == 'u')
                    {
                        uint32_t dec = va_arg(args, uint32_t);
                        char str[16];
                        memset(str, 0, 16);
                        print(ltoa(dec, str, 10));
                    }
                    else if (*fmt == 'x')
                    {
                        uint32_t dec = va_arg(args, uint32_t);
                        char str[16];
                        memset(str, 0, 16);
                        print(ltoa(dec, str, 16));
                    }
                    else if (*fmt == '2')
                    {
                        fmt++;
                        if (*fmt != 'x' && *fmt != 'X') { write(*fmt); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            print(strhex(num, str, false, 1));
                        }
                    }
                    else if (*fmt == '4')
                    {
                        fmt++;
                        if (*fmt != 'x' && *fmt != 'X') { write(*fmt); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            print(strhex(num, str, false, 2));
                        }
                    }
                    else if (*fmt == '8')
                    {
                        fmt++;
                        if (*fmt != 'x' && *fmt != 'X') { write(*fmt); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            print(strhex(num, str, false, 4));
                        }
                    }
                    else if (*fmt == 'f') { }
                    else if (*fmt == 's')
                    {
                        char* val = va_arg(args, char*);
                        print(val);
                    }
                    else { write(*fmt); }
                }
                else { write(*fmt); }
                fmt++;
            }
        }

        void serial_controller::print_fmt(char* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            vprintf(fmt, args);
            va_end(args);
        }

        bool serial_controller::can_send() { return ports::inb((uint16_t)this->_port + 5) & 0x20; }

        bool serial_controller::has_recv() { return ports::inb((uint16_t)this->_port + 5) & 1; }

        bool serial_controller::validate()
        {
            if (this->_port == serial_port::com1) { return true; }
            if (this->_port == serial_port::com2) { return true; }
            if (this->_port == serial_port::com3) { return true; }
            if (this->_port == serial_port::com4) { return true; }
            return false;
        }

        const char* serial_controller::portstr(serial_port port)
        {
            switch (port)
            {
                default:                { return serial_portnames[0]; }
                case serial_port::com1: { return serial_portnames[1]; }
                case serial_port::com2: { return serial_portnames[2]; }
                case serial_port::com3: { return serial_portnames[3]; }
                case serial_port::com4: { return serial_portnames[4]; }
            }
        }

        serial_port serial_controller::port() { return this->_port; }
    }
}   