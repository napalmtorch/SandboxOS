#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

namespace os
{
    namespace hal
    {
        /// @brief Available serial ports
        enum class serial_port : uint16_t
        {
            /// @brief Disabled
            disabled = 0x000,
            /// @brief COM1(0x3F8)
            com1     = 0x3F8,
            /// @brief COM2(0x2F8)
            com2     = 0x2F8,
            /// @brief COM3(0x3E8)
            com3     = 0x3E8,
            /// @brief COM4(0x2E8)
            com4     = 0x2E8,
        };

        /// @brief Serial port names
        static const char* serial_portnames[5] = 
        {
            "disabled",
            "com1",
            "com2",
            "com3",
            "com4",
        };

        /// @brief Serial interface controller
        class serial_controller
        {
            private:
                /// @brief Active serial port
                serial_port _port;

            public:
                /// @brief Begin serial connection on specified port @param port Serial port number
                void begin(serial_port port);
                /// @brief Disconnect serial from active port
                void end();

            public:
                /// @brief Read byte from serial port @return 8-bit value
                char read();
                /// @brief Write byte to serial port @param c 8-bit value
                void write(char c);
                /// @brief Write string to serial port @param str Pointer to string
                void print(char* str);
                /// @brief Write string and line feed to serial port @param str Pointer to string
                void println(char* str);
                /// @brief Write formatted string with specified variadic arguments to serial port @param fmt Pointer to string @param args Variadic argument list
                void vprint_fmt(char* fmt, va_list args);
                /// @brief Write formatted string to serial port @param fmt Pointer to string @param ... Arguments
                void print_fmt(char* fmt, ...);

            private:
                /// @brief Check if serial is able to send data @return Sending allowed
                bool can_send();
                /// @brief Check is serial port has received data @return Data received
                bool has_recv();
                /// @brief Check if serial port is valid @return Active serial port is not disabled
                bool validate();

            public:
                /// @brief Get port name from specified port number @param port Serial port @return Pointer to string
                static const char* portstr(serial_port port);
                /// @brief Get active serial port @return Serial port number
                serial_port port();
        };
    }
}