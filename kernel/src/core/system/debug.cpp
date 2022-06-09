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

        void debug::print_regs(hal::idt_registers_t* regs)
        {
            printf("EAX: 0x%8x EBX: 0x%8x ECX: 0x%8x EDX: 0x%8x\n", regs->eax, regs->ebx, regs->ecx, regs->edx);
            printf("ESI: 0x%8x EDI: 0x%8x EBP: 0x%8x ESP: 0x%8x\n", regs->esi, regs->edi, regs->ebp, regs->esp);
            printf("EIP: 0x%8x EFL: 0x%8x CS:  0x%8x SS:  0x%8x\n", regs->eip, regs->eflags, regs->cs, regs->ss);
            printf("CR0: 0x%8x CR2: 0x%8x CR3: 0x%8x CR4: 0x%8x\n", _read_cr0(), _read_cr2(), _read_cr3(), _read_cr4());
        }

        void debug::print_regs(threading::thread_regs_t* regs)
        {
            printf("EAX: 0x%8x EBX: 0x%8x ECX: 0x%8x EDX: 0x%8x\n", regs->eax, regs->ebx, regs->ecx, regs->edx);
            printf("ESI: 0x%8x EDI: 0x%8x EBP: 0x%8x ESP: 0x%8x\n", regs->esi, regs->edi, regs->ebp, regs->esp);
            printf("EFL: 0x%8x CS:  0x%8x SS:  0x%8x\n", regs->eflags);
            printf("CR0: 0x%8x CR2: 0x%8x CR3: 0x%8x CR4: 0x%8x\n", _read_cr0(), _read_cr2(), _read_cr3(), _read_cr4());
        }
    }
}