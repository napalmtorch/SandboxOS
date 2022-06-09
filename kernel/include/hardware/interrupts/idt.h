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

        /// @brief Interrupt numbers
        enum class IRQ : uint8_t
        {
            /// @brief Programmable interval timer
            IRQ0 = 32,
            /// @brief PS/2 Keyboard
            IRQ1,
            /// @brief Cascade
            IRQ2,
            /// @brief Serial COM2 (if enabled)
            IRQ3,
            /// @brief Serial COM1 (if enabled)
            IRQ4,
            /// @brief LPT2 (if enabled)
            IRQ5,
            /// @brief Floppy disk
            IRQ6,
            /// @brief LPT1 (unreliable)
            IRQ7,
            /// @brief CMOS real-time clock (if enabled)
            IRQ8,
            /// @brief Free for peripherals/legacy SCSI/NIC
            IRQ9,
            /// @brief Free for peripherals/SCSI/NIC
            IRQ10,
            /// @brief Free for peripherals/SCSI/NIC
            IRQ11,
            /// @brief PS/2 Mouse
            IRQ12,
            /// @brief FPU/Coprocessor/Inter-processor
            IRQ13,
            /// @brief Primary ATA hard disk
            IRQ14,
            /// @brief Secondary ATA hard disk
            IRQ15,
            /// @brief System call
            IRQ_SYSCALL = 160,
        };
        
        /// @brief Interrupt descriptor types
        typedef enum
        {
            /// @brief Task
            IDTTYPE_TASK   = 0x05,
            /// @brief 16-bit interrupt
            IDTTYPE_INT16  = 0x06,
            /// @brief 32-bit interrupt
            IDTTYPE_INT32  = 0x0E,
            /// @brief 16-bit trap
            IDTTYPE_TRAP16 = 0x07,
            /// @brief 32-bit trap
            IDTTYPE_TRAP32 = 0x0F,
        } IDTTYPE;

        /// @brief Structure for managing interrupt descriptor flags
        typedef struct
        {
            /// @brief Interrupt descriptor type
            IDTTYPE type    : 4;
            /// @brief Unused/0
            uint8_t unused  : 1;
            /// @brief Privilege level
            uint8_t dpl     : 2;
            /// @brief Present
            uint8_t present : 1;
        } PACKED idt_flags_t;

        /// @brief Interrupt descriptor table entry
        typedef struct
        {
            /// @brief Offset bits 0-15
            uint16_t offset_low;
            /// @brief Selector segment
            uint16_t segment;
            /// @brief Reserved
            uint8_t reserved;
            /// @brief Flags
            idt_flags_t flags;
            /// @brief Offset bits 16-31
            uint16_t offset_high;
        } PACKED idt_entry_t;

        /// @brief Interrupt descriptor table register
        typedef struct
        {
            /// @brief Address limit
            uint16_t limit;
            /// @brief Base address
            uint32_t base;
        } PACKED idtr_t;

        /// @brief Structure for managing interrupt registers
        typedef struct
        {
            uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
            uint32_t irq, error;
            uint32_t eip, cs, eflags, useresp, ss;
        } PACKED idt_registers_t;

        /// @internal Interrupt callback handler
        typedef void (*idt_handler_t)(idt_registers_t* regs);

        /// @brief Interrupt descriptor table
        namespace idt
        {
            /// @brief Initialize interrupt descriptor table
            void init();
            /// @brief Initialize interrupt exception descriptors
            void init_exceptions();
            /// @brief Initialize interrupt irq descriptors
            void init_irqs();
            /// @brief Set specified interrupt descriptor entry @param n Entry index @param base Base address @param flags Descriptor flags @param seg Selector segment
            void set_descriptor(uint8_t n, uint32_t base, idt_flags_t flags, uint16_t seg);
            /// @brief Register interrupt callback handler @param irq Interrupt number @param handler Pointer to interrupt callback
            void register_irq(IRQ irq, idt_handler_t handler);
            /// @brief Un-register interrupt callback handler @param irq Interrupt number
            void unregister_irq(IRQ irq);
            /// @brief Acknowledge interrupt request @param regs Pointer to interrupt registers
            void ack_irq(idt_registers_t* regs);
            /// @brief Check if interrupts are enabled
            bool irqs_enabled();
        }
    }
}