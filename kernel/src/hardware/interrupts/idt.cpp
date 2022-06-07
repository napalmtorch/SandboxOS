#include <hardware/interrupts/idt.h>
#include <core/kernel.h>

#define IDT_COUNT 256

EXTC
{
    #define isr(x) extern void isr##x();
    #define irq(x) extern void irq##x();

    isr(0);  isr(1);  isr(2);  isr(3);  isr(4);  isr(5);  isr(6);  isr(7);
    isr(8);  isr(9);  isr(10); isr(11); isr(12); isr(13); isr(14); isr(15);
    isr(16); isr(17); isr(18); isr(19); isr(20); isr(21); isr(22); isr(23);
    isr(24); isr(25); isr(26); isr(27); isr(28); isr(29); isr(30); isr(31);

    irq(0);   irq(1); irq(2);  irq(3);  irq(4);  irq(5);  irq(6);  irq(7);
    irq(8);   irq(9); irq(10); irq(11); irq(12); irq(13); irq(14); irq(15);
    irq(128); 

    extern void _idt_flush(uint32_t idt);

    os::hal::idt_flags_t IDT_EXCEPTION_ATTRS = (os::hal::idt_flags_t) { .type = os::hal::IDTTYPE_TRAP32, .unused = 0, .dpl = 3, .present = 1 };
    os::hal::idt_flags_t IDT_IRQ_ATTRS       = (os::hal::idt_flags_t) { .type = os::hal::IDTTYPE_INT32,  .unused = 0, .dpl = 3, .present = 1 };

    os::hal::idtr_t        _idt_reg                 ALIGNED(0x100);
    os::hal::idt_entry_t   _idt_entries[IDT_COUNT]  ALIGNED(0x100);
    os::hal::idt_handler_t _idt_handlers[IDT_COUNT] ALIGNED(0x100);

    /// @brief Handle exception interrupts @param regs Pointer to registers
    void exception_handler(os::hal::idt_registers_t* regs)
    {
        asm volatile("cli");
        const int count = sizeof(os::hal::EXCEPTION_MSGS) / sizeof(char*);
        if (regs->irq >= count) { printf("%s Unknown Exception\n", DEBUG_ERROR); }
        else { printf("%s %s Exception\n", DEBUG_ERROR, os::hal::EXCEPTION_MSGS[regs->irq]); }

        printf("EAX: 0x%8x EBX: 0x%8x ECX: 0x%8x EDX: 0x%8x\n", regs->eax, regs->ebx, regs->ecx, regs->edx);
        printf("ESI: 0x%8x EDI: 0x%8x EBP: 0x%8x ESP: 0x%8x\n", regs->esi, regs->edi, regs->ebp, regs->esp);
        printf("EIP: 0x%8x EFL: 0x%8x CS:  0x%8x SS:  0x%8x\n", regs->eip, regs->eflags, regs->cs, regs->ss);
        printf("CR0: 0x%8x CR2: 0x%8x CR3: 0x%8x CR4: 0x%8x\n", _read_cr0(), _read_cr2(), _read_cr3(), _read_cr4());
        asm volatile("hlt");
    }

    /// @brief Handle IRQ interrupts @param regs Pointer to registers
    void irq_handler(os::hal::idt_registers_t* regs)
    {
        if (_idt_handlers[regs->irq] != NULL) { _idt_handlers[regs->irq](regs); }
        else
        {
            if (regs->irq != (uint32_t)os::hal::IRQ::IRQ0) { printf("%s Unhandled interrupt 0x%2x\n", DEBUG_WARN, regs->irq); }
            os::hal::idt::ack_irq(regs);
        }
    }

    void default_handler(os::hal::idt_registers_t* regs) { }
}

namespace os
{
    namespace hal
    {
        namespace idt
        {
            /// @brief Initialize interrupt descriptor table
            void init()
            {
                // clear tables
                memset(&_idt_entries,  0, IDT_COUNT * sizeof(idt_entry_t));
                memset(&_idt_handlers, 0, IDT_COUNT * sizeof(idt_handler_t));

                // initialize exception interrupts
                init_exceptions();
                
                // re-map PIC
                pic::remap();

                // initialize irq interrupts
                init_irqs();

                // setup descriptor table
                _idt_reg.base  = (uint32_t)&_idt_entries;
                _idt_reg.limit = (sizeof(idt_entry_t) * IDT_COUNT) - 1;
                
                // set idt location and enable interrupts
                _idt_flush((uint32_t)&_idt_reg);
                asm volatile("sti");

                // interrupt 7 can sometimes fire randomly - provide default handler to prevent
                register_irq(IRQ::IRQ7, default_handler);

                // finished
                printf("%s Initialized IDT - ADDR:0x%8x\n", DEBUG_OK, (uint32_t)&_idt_reg);
            }

            /// @brief Initialize exception interrupt descriptors
            void init_exceptions()
            {
                set_descriptor(0x00, (uint32_t)isr0,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x01, (uint32_t)isr1,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x02, (uint32_t)isr2,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x03, (uint32_t)isr3,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x04, (uint32_t)isr4,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x05, (uint32_t)isr5,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x06, (uint32_t)isr6,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x07, (uint32_t)isr7,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x08, (uint32_t)isr8,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x09, (uint32_t)isr9,  IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x0A, (uint32_t)isr10, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x0B, (uint32_t)isr11, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x0C, (uint32_t)isr12, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x0D, (uint32_t)isr13, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x0E, (uint32_t)isr14, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x0F, (uint32_t)isr15, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x10, (uint32_t)isr16, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x11, (uint32_t)isr17, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x12, (uint32_t)isr18, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x13, (uint32_t)isr19, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x14, (uint32_t)isr20, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x15, (uint32_t)isr21, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x16, (uint32_t)isr22, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x17, (uint32_t)isr23, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x18, (uint32_t)isr24, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x19, (uint32_t)isr25, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x1A, (uint32_t)isr26, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x1B, (uint32_t)isr27, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x1C, (uint32_t)isr28, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x1D, (uint32_t)isr29, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x1E, (uint32_t)isr30, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                set_descriptor(0x1F, (uint32_t)isr31, IDT_EXCEPTION_ATTRS, KERNEL_CS);
                printf("%s Initialized exception descriptors\n", DEBUG_INFO);
            }

            /// @brief Initialize IRQ interrupt descriptors
            void init_irqs()
            {
                set_descriptor(0x20, (uint32_t)irq0,  IDT_IRQ_ATTRS, KERNEL_CS);
                set_descriptor(0x21, (uint32_t)irq1,  IDT_IRQ_ATTRS, KERNEL_CS);
                set_descriptor(0x22, (uint32_t)irq2,  IDT_IRQ_ATTRS, KERNEL_CS);
                set_descriptor(0x23, (uint32_t)irq3,  IDT_IRQ_ATTRS, KERNEL_CS);
                set_descriptor(0x24, (uint32_t)irq4,  IDT_IRQ_ATTRS, KERNEL_CS);
                set_descriptor(0x25, (uint32_t)irq5,  IDT_IRQ_ATTRS, KERNEL_CS);
                set_descriptor(0x26, (uint32_t)irq6,  IDT_IRQ_ATTRS, KERNEL_CS);
                set_descriptor(0x27, (uint32_t)irq7,  IDT_IRQ_ATTRS, KERNEL_CS);
                set_descriptor(0x28, (uint32_t)irq8,  IDT_IRQ_ATTRS, KERNEL_CS);
                set_descriptor(0x29, (uint32_t)irq9,  IDT_IRQ_ATTRS, KERNEL_CS);
                set_descriptor(0x2A, (uint32_t)irq10, IDT_IRQ_ATTRS, KERNEL_CS);
                set_descriptor(0x2B, (uint32_t)irq11, IDT_IRQ_ATTRS, KERNEL_CS);
                set_descriptor(0x2C, (uint32_t)irq12, IDT_IRQ_ATTRS, KERNEL_CS);
                set_descriptor(0x2D, (uint32_t)irq13, IDT_IRQ_ATTRS, KERNEL_CS);
                set_descriptor(0x2E, (uint32_t)irq14, IDT_IRQ_ATTRS, KERNEL_CS);
                set_descriptor(0x2F, (uint32_t)irq15, IDT_IRQ_ATTRS, KERNEL_CS);
                set_descriptor(0x80, (uint32_t)irq128, IDT_IRQ_ATTRS, KERNEL_CS);
                printf("%s Initialized irq descriptors\n", DEBUG_INFO);
            }

            /// @brief Set specified interrupt descriptor @param n Descriptor index @param base Base address @param flags Flags @param seg Segment
            void set_descriptor(uint8_t n, uint32_t base, idt_flags_t flags, uint16_t seg)
            {
                _idt_entries[n].offset_low    = LOW_16(base);
                _idt_entries[n].offset_high   = HIGH_16(base);
                _idt_entries[n].reserved      = 0;
                _idt_entries[n].flags.type    = flags.type;
                _idt_entries[n].flags.dpl     = flags.dpl;
                _idt_entries[n].flags.present = flags.present;
                _idt_entries[n].flags.unused  = 0;
                _idt_entries[n].segment       = seg;
            }

            /// @brief Assign callback function to specified IRQ @param irq IRQ number @param handler Callback function handler
            void register_irq(IRQ irq, idt_handler_t handler)
            {
                if (handler == NULL) { perror("Tried to register null interrupt handler\n"); }
                _idt_handlers[(uint8_t)irq] = handler;
                printf("%s Registered handler at 0x%8x to irq 0x%2x\n", DEBUG_INFO, handler, irq);
            }

            /// @brief Unassign function from specified IRQ @param irq IRQ number
            void unregister_irq(IRQ irq)
            {
                if (_idt_handlers[(uint8_t)irq] == NULL) { return; }
                _idt_handlers[(uint8_t)irq] = NULL;
                printf("%s Unregistered handler for irq 0x%2x\n", DEBUG_INFO, irq);
            }

            /// @brief Acknowledge that interrupt has been handled @param regs Pointer to callback registers
            void ack_irq(idt_registers_t* regs)
            {
                if (regs->irq >= 40) { ports::outb(0xA0, 0x20); }
                ports::outb(0x20, 0x20);
            }
        }
    }
}