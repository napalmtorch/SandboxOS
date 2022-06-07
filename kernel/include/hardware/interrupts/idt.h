#pragma once
#include <stdint.h>

namespace os
{
    namespace hal
    {
        static const char* EXCEPTION_MSGS[] = 
        {
            "Divide By Zero",
            "Debug",
            "Non-maskable Interrupt",
            "Breakpoint",
            "Overflow",
            "Bound Range Exceeded",
            "Invalid Opcode",
            "Device Not Available",
            "Double Fault",
            "Co-processor Segment Overrun",
            "Invalid TSS",
            "Segment Not Present",
            "Stack-Segment Fault",
            "General Protection Fault",
            "Page Fault",
            "Reserved",
            "x87 Floating-Point Exception",
            "Alignment Check",
            "Machine Check",
            "SIMD Floating-Point Exception",
            "Virtualization Exception",
            "Control Protection Exception",
            "Reserved",
            "Hypervisor Injection Exception",
            "VMM Communication Exception",
            "Security Exception",
            "Reserved",
            "Triple Fault",
            "FPU Error Interrupt",
        };

        enum class IRQ : uint8_t
        {
            IRQ0 = 32,
            IRQ1,
            IRQ2,
            IRQ3,
            IRQ4,
            IRQ5,
            IRQ6,
            IRQ7,
            IRQ8,
            IRQ9,
            IRQ10,
            IRQ11,
            IRQ12,
            IRQ13,
            IRQ14,
            IRQ15,
            IRQ_SYSCALL = 160,
        };
        
        typedef enum
        {
            IDTTYPE_TASK   = 0x05,
            IDTTYPE_INT16  = 0x06,
            IDTTYPE_INT32  = 0x0E,
            IDTTYPE_TRAP16 = 0x07,
            IDTTYPE_TRAP32 = 0x0F,
        } IDTTYPE;

        typedef struct
        {
            IDTTYPE type    : 4;
            uint8_t unused  : 1;
            uint8_t dpl     : 2;
            uint8_t present : 1;
        } PACKED idt_flags_t;

        typedef struct
        {
            uint16_t    offset_low;
            uint16_t    segment;
            uint8_t     reserved;
            idt_flags_t flags;
            uint16_t    offset_high;
        } PACKED idt_entry_t;

        typedef struct
        {
            uint16_t limit;
            uint32_t base;
        } PACKED idtr_t;

        typedef struct
        {
            uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
            uint32_t irq, error;
            uint32_t eip, cs, eflags, useresp, ss;
        } PACKED idt_registers_t;

        typedef void (*idt_handler_t)(idt_registers_t* regs);

        namespace idt
        {
            void init();
            void init_exceptions();
            void init_irqs();
            void set_descriptor(uint8_t n, uint32_t base, idt_flags_t flags, uint16_t seg);
            void register_irq(IRQ irq, idt_handler_t handler);
            void unregister_irq(IRQ irq);
            void ack_irq(idt_registers_t* regs);
        }
    }
}