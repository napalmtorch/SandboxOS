#include <core/system/debug.h>
#include <core/kernel.h>

namespace os
{
    namespace sys
    {
        hal::serial_controller* debug::serial;
        hal::serial_controller  debug::_dbg_serial;

        bool debug::stdout_enabled = false;

        void debug::init(hal::serial_port port)
        {
            stdout_enabled = true;
            serial = &_dbg_serial;
            _dbg_serial.init();
            _dbg_serial.start(port);
            _dbg_serial.print_fmt("%s Enabled serial debugging on %s\n", DEBUG_OK, serial->portstr(port));
        }

        void debug::disable()
        {
            stdout_enabled = false;
            serial->stop();
        }

        void debug::print_regs(hal::idt_registers_t* regs)
        {
            if (regs == NULL) { return; }
            printf("EAX: 0x%8x EBX: 0x%8x ECX: 0x%8x EDX: 0x%8x\n", regs->eax, regs->ebx, regs->ecx, regs->edx);
            printf("ESI: 0x%8x EDI: 0x%8x EBP: 0x%8x ESP: 0x%8x\n", regs->esi, regs->edi, regs->ebp, regs->esp);
            printf("EIP: 0x%8x EFL: 0x%8x CS:  0x%8x SS:  0x%8x\n", regs->eip, regs->eflags, regs->cs, regs->ss);
            printf("CR0: 0x%8x CR2: 0x%8x CR3: 0x%8x CR4: 0x%8x\n", _read_cr0(), _read_cr2(), _read_cr3(), _read_cr4());
        }

        void debug::print_regs(threading::thread_regs_t* regs)
        {
            if (regs == NULL) { return; }
            printf("EAX: 0x%8x EBX: 0x%8x ECX: 0x%8x EDX: 0x%8x\n", regs->eax, regs->ebx, regs->ecx, regs->edx);
            printf("ESI: 0x%8x EDI: 0x%8x EBP: 0x%8x ESP: 0x%8x\n", regs->esi, regs->edi, regs->ebp, regs->esp);
            printf("EFL: 0x%8x CS:  0x%8x SS:  0x%8x\n", regs->eflags);
            printf("CR0: 0x%8x CR2: 0x%8x CR3: 0x%8x CR4: 0x%8x\n", _read_cr0(), _read_cr2(), _read_cr3(), _read_cr4());
        }

        void debug::draw_overlay(std::gfx::image* img)
        {
            int yy = 0;
            char temp[64];
            char temp2[64];

            // current time
            memset(temp, 0, sizeof(temp));
            memset(temp2, 0, sizeof(temp2));
            std::time_t now = std::timenow();
            std::timestr(now, temp, std::time_format::standard, true);
            strcat(temp2, "TIME      - ");
            strcat(temp2, temp);
            img->putstr(0, 15 * yy++, temp2, std::FONT_DEFAULT, 0xFF00FF00, 0xFF000000);

            // uptime
            memset(temp, 0, sizeof(temp));
            memset(temp2, 0, sizeof(temp2));
            strcat(temp2, "UPTIME    - ");
            strcat(temp2, ltoa(hal::pit::seconds(), temp, 10));
            strcat(temp2, "s");
            img->putstr(0, 15 * yy++, temp2, std::FONT_DEFAULT, 0xFF00FF00, 0xFF000000);

            // threads
            memset(temp, 0, sizeof(temp));
            memset(temp2, 0, sizeof(temp2));
            strcat(temp2, "THREADS   - ");
            strcat(temp2, ltoa(threading::scheduler::threads()->length(), temp, 10));
            img->putstr(0, 15 * yy++, temp2, std::FONT_DEFAULT, 0xFF00FF00, 0xFF000000);

            // fps
            memset(temp, 0, sizeof(temp));
            memset(temp2, 0, sizeof(temp2));
            strcat(temp2, "FPS       - ");
            strcat(temp2, ltoa(kernel::shell->fps(), temp, 10));
            img->putstr(0, 15 * yy++, temp2, std::FONT_DEFAULT, 0xFF00FF00, 0xFF000000);

            // memory usage
            memset(temp, 0, sizeof(temp));
            memset(temp2, 0, sizeof(temp2));
            strcat(temp2, "RAM USAGE - ");
            strcat(temp2, ltoa((kernel::heap_large.calc_used() + kernel::heap_small.calc_used()) / KB, temp, 10));
            strcat(temp2, "/");
            memset(temp, 0, sizeof(temp));
            strcat(temp2, ltoa((kernel::heap_large.data_size() + kernel::heap_small.data_size()) / KB, temp, 10));
            strcat(temp2, " KB");
            img->putstr(0, 15 * yy++, temp2, std::FONT_DEFAULT, 0xFF00FF00, 0xFF000000);

            // kernel
            img->putstr(0, 15 * yy, "KERNEL    - ", std::FONT_DEFAULT, 0xFF00FF00, 0xFF000000);
            memset(temp, 0, sizeof(temp));
            memset(temp2, 0, sizeof(temp2));
            strcat(temp2, "CPU: ");
            if (kernel::thread->time.cpu_usage < 10) { stradd(temp2, '0'); }
            strcat(temp2, ltoa(kernel::thread->time.cpu_usage, temp, 10));            
            strcat(temp2, "% MEM: ");
            memset(temp, 0, sizeof(temp));
            strcat(temp2, ltoa(kernel::heap_large.calc_used(kernel::thread) + kernel::heap_small.calc_used(kernel::thread), temp, 10));
            strcat(temp2, " bytes");
            img->putstr(108, 15 * yy++, temp2, std::FONT_DEFAULT, 0xFF00FF00, 0xFF000000);

            // gc
            if (garbage_collector::thread != NULL)
            {
                img->putstr(0, 15 * yy, "GCOLLECT  - ", std::FONT_DEFAULT, 0xFF00FF00, 0xFF000000);
                memset(temp, 0, sizeof(temp));
                memset(temp2, 0, sizeof(temp2));
                strcat(temp2, "CPU: ");
                if (garbage_collector::thread->time.cpu_usage < 10) { stradd(temp2, '0'); }
                strcat(temp2, ltoa(garbage_collector::thread->time.cpu_usage, temp, 10));
                strcat(temp2, "% MEM: ");
                memset(temp, 0, sizeof(temp));
                strcat(temp2, ltoa(kernel::heap_large.calc_used(garbage_collector::thread) + kernel::heap_small.calc_used(garbage_collector::thread), temp, 10));
                strcat(temp2, " bytes");
                img->putstr(108, 15 * yy++, temp2, std::FONT_DEFAULT, 0xFF00FF00, 0xFF000000);
            }

            // shell
            img->putstr(0, 15 * yy, "SHELL     - ", std::FONT_DEFAULT, 0xFF00FF00, 0xFF000000);
            memset(temp, 0, sizeof(temp));
            memset(temp2, 0, sizeof(temp2));
            strcat(temp2, "CPU: ");
            if (kernel::shell->thread->time.cpu_usage < 10) { stradd(temp2, '0'); }
            strcat(temp2, ltoa(kernel::shell->thread->time.cpu_usage, temp, 10));
            strcat(temp2, "% MEM: ");
            memset(temp, 0, sizeof(temp));
            strcat(temp2, ltoa(kernel::heap_large.calc_used(kernel::shell->thread) + kernel::heap_small.calc_used(kernel::shell->thread), temp, 10));
            strcat(temp2, " bytes");
            img->putstr(108, 15 * yy++, temp2, std::FONT_DEFAULT, 0xFF00FF00, 0xFF000000);

            // cmd handler
            img->putstr(0, 15 * yy, "CMDHOST   - ", std::FONT_DEFAULT, 0xFF00FF00, 0xFF000000);
            memset(temp, 0, sizeof(temp));
            memset(temp2, 0, sizeof(temp2));
            strcat(temp2, "CPU: ");
            if (sys::command_handler::thread->time.cpu_usage < 10) { stradd(temp2, '0'); }
            strcat(temp2, ltoa(sys::command_handler::thread->time.cpu_usage, temp, 10));
            strcat(temp2, "% MEM: ");
            memset(temp, 0, sizeof(temp));
            strcat(temp2, ltoa(kernel::heap_large.calc_used(sys::command_handler::thread) + kernel::heap_small.calc_used(sys::command_handler::thread), temp, 10));
            strcat(temp2, " bytes");
            img->putstr(108, 15 * yy++, temp2, std::FONT_DEFAULT, 0xFF00FF00, 0xFF000000);
        }
    }
}