#include <hardware/device.h>
#include <core/kernel.h>

namespace os
{
    namespace hal
    {
        void device_t::init()
        {
            _running = false;
        }

        void device_t::start()
        {
            if (_running) { return; }
            _running = true;
        }

        void device_t::stop()
        {
            if (!_running) { return; }
            _running = false;
        }

        void device_t::set_info(char* name, uint32_t id)
        {
            strcpy(this->_name, name);
            this->_id = id;
        }
    
        uint32_t device_t::id() { return this->_id; }

        char* device_t::name() { return this->_name; }       

        bool device_t::running() { return this->_running; }
    }
}