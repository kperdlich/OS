#include "gdt.h"
#include "../IO.h"
#include "../vga.h"

#define HALT asm("cli; hlt");

#define NAKED __attribute__((naked))

#define EXCEPTION_HANDLER(idx, msg)       \
    static void exception_handler_##idx() \
    {                                     \
        terminal_writestring(msg);        \
        HALT                              \
    }

EXCEPTION_HANDLER(0, "Division by zero")
EXCEPTION_HANDLER(1, "Debug exception")
EXCEPTION_HANDLER(2, "Non maskable interrupt")
EXCEPTION_HANDLER(3, "Breakpoint")
EXCEPTION_HANDLER(4, "Detected overflow")
EXCEPTION_HANDLER(5, "Out of bounds")
EXCEPTION_HANDLER(6, "Invalid opcode")
EXCEPTION_HANDLER(7, "No coprocessor")
EXCEPTION_HANDLER(8, "Double fault")
EXCEPTION_HANDLER(9, "Coprocessor segment overrun")
EXCEPTION_HANDLER(10, "Bad TSS")
EXCEPTION_HANDLER(11, "Segment not present")
EXCEPTION_HANDLER(12, "Stack fault")
EXCEPTION_HANDLER(13, "General protection fault")
EXCEPTION_HANDLER(14, "Page fault ")
EXCEPTION_HANDLER(15, "Unknown interrupt exception")
EXCEPTION_HANDLER(16, "Coprocessor fault")
EXCEPTION_HANDLER(17, "Alignment check exception")
EXCEPTION_HANDLER(18, "Machine check exception")
EXCEPTION_HANDLER(19_31, "Exception 19 - 31")

/*#define IRQ_STUB(idx)                     \
    static void irq_handler_##idx()       \
    {                                     \
        terminal_writestring("IRQ_STUB"); \
                                          \
        if (idx >= 40) {                  \
            IO::outb(0xA0, 0x20);         \
        }                                 \
        IO::outb(0x20, 0x20);             \
    }
IRQ_STUB(0)
IRQ_STUB(1)
IRQ_STUB(2)
IRQ_STUB(3)
IRQ_STUB(4)
IRQ_STUB(5)
IRQ_STUB(6)
IRQ_STUB(7)
IRQ_STUB(8)
IRQ_STUB(9)
IRQ_STUB(10)
IRQ_STUB(11)
IRQ_STUB(12)
IRQ_STUB(13)
IRQ_STUB(14)
IRQ_STUB(15)*/

extern "C" void Irq_handler();
extern "C" void ISR_ENTRY();

asm(
    ".globl ISR_ENTRY \n"
    "ISR_ENTRY: \n"
    "    pusha\n"
    "    pushw %ds\n"
    "    pushw %es\n"
    "    pushw %ss\n"
    "    pushw %ss\n"
    "    popw %ds\n"
    "    popw %es\n"
    "    call Irq_handler\n"
    "    popw %es\n"
    "    popw %ds\n"
    "    popa\n"
    "    iret\n");

void Irq_handler()
{
    terminal_writestring("IRQ_STUB");

    IO::outb(0x20, 0x20);
}

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

// Internal function prototypes.
static void init_gdt();
static void gdt_set_gate(s32int, u32int, u32int, u8int, u8int);

static void init_idt();
static void idt_set_gate(u8int, void(handler)(), u16int, u8int);

void init_timer(u32int frequency);

void init_descriptor_tables()
{
    // Initialise the global descriptor table.
    init_gdt();
    init_idt();
}

static void init_gdt()
{
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base = (u32int)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0); // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel Code segment    10011010
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel Data segment    10010010
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment 11111010
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment 11110010

    asm volatile("lgdtl %0" ::"m"(gdt_ptr));

    asm volatile("mov %0, %%ds\n"
                 "mov %0, %%es\n"
                 "mov %0, %%fs\n"
                 "mov %0, %%gs\n"
                 "mov %0, %%ss\n"
                 "ljmp $0x08, $.FarJump\n" // Updating cs requires far jump; 0x08 is the offset to our kernel code segment
                 ".FarJump:\n" ::"r"(0x10)); // 0x10 is the offset in the GDT to our kernel data segment
}

// Set the value of one GDT entry.
static void gdt_set_gate(s32int num, u32int base, u32int limit, u8int access, u8int gran)
{
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access = access;
}

static void init_idt()
{
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (u32int)&idt_entries;

    __builtin_memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

    // Remap the irq table. http://www.jamesmolloy.co.uk/tutorial_html/5.-IRQs%20and%20the%20PIT.html
    IO::outb(0x20, 0x11); /* write ICW1 to PICMaster, we are gonna write commands to PICMaster */
    IO::outb(0xA0, 0x11); /* write ICW1 to PICSlave, we are gonna write commands to PICSlave */
    IO::outb(0x21, 0x20); /* remap PICMaster to 0x20 (32 decimal) */
    IO::outb(0xA1, 0x28); /* remap PICSlave to 0x28 (40 decimal) */
    IO::outb(0x21, 0x04); /* IRQ2 -> connection to slave */
    IO::outb(0xA1, 0x02);
    IO::outb(0x21, 0x01); /* write ICW4 to PICMaster, we are gonna write commands to PICMaster */
    IO::outb(0xA1, 0x01); /* write ICW4 to PICSlave, we are gonna write commands to PICSlave */
    IO::outb(0x21, 0x0); /* enable all IRQs on PICMaster */
    IO::outb(0xA1, 0x0); /* enable all IRQs on PICSlave */

    idt_set_gate(0, exception_handler_0, 0x08, 0x8E);
    idt_set_gate(1, exception_handler_1, 0x08, 0x8E);
    idt_set_gate(2, exception_handler_2, 0x08, 0x8E);
    idt_set_gate(3, exception_handler_3, 0x08, 0x8E);
    idt_set_gate(4, exception_handler_4, 0x08, 0x8E);
    idt_set_gate(5, exception_handler_5, 0x08, 0x8E);
    idt_set_gate(6, exception_handler_6, 0x08, 0x8E);
    idt_set_gate(7, exception_handler_7, 0x08, 0x8E);
    idt_set_gate(8, exception_handler_8, 0x08, 0x8E);
    idt_set_gate(9, exception_handler_9, 0x08, 0x8E);
    idt_set_gate(10, exception_handler_10, 0x08, 0x8E);
    idt_set_gate(11, exception_handler_11, 0x08, 0x8E);
    idt_set_gate(12, exception_handler_17, 0x08, 0x8E);
    idt_set_gate(13, exception_handler_13, 0x08, 0x8E);
    idt_set_gate(14, exception_handler_14, 0x08, 0x8E);
    idt_set_gate(15, exception_handler_15, 0x08, 0x8E);
    idt_set_gate(16, exception_handler_16, 0x08, 0x8E);
    idt_set_gate(17, exception_handler_17, 0x08, 0x8E);
    idt_set_gate(18, exception_handler_18, 0x08, 0x8E);

    for (int i = 19; i <= 31; ++i) {
        idt_set_gate(i, exception_handler_19_31, 0x08, 0x8E);
    }

    idt_set_gate(32, ISR_ENTRY, 0x08, 0x8E);
    /*idt_set_gate(33, ISR_ENTRY, 0x08, 0x8E);
    idt_set_gate(34, ISR_ENTRY, 0x08, 0x8E);
    idt_set_gate(35, ISR_ENTRY, 0x08, 0x8E);
    idt_set_gate(36, ISR_ENTRY, 0x08, 0x8E);
    idt_set_gate(37, ISR_ENTRY, 0x08, 0x8E);
    idt_set_gate(38, ISR_ENTRY, 0x08, 0x8E);
    idt_set_gate(39, ISR_ENTRY, 0x08, 0x8E);
    idt_set_gate(40, ISR_ENTRY, 0x08, 0x8E);
    idt_set_gate(41, ISR_ENTRY, 0x08, 0x8E);
    idt_set_gate(42, ISR_ENTRY, 0x08, 0x8E);
    idt_set_gate(43, ISR_ENTRY, 0x08, 0x8E);
    idt_set_gate(44, ISR_ENTRY, 0x08, 0x8E);
    idt_set_gate(45, ISR_ENTRY, 0x08, 0x8E);
    idt_set_gate(46, ISR_ENTRY, 0x08, 0x8E);
    idt_set_gate(47, ISR_ENTRY, 0x08, 0x8E);*/

    asm volatile("lidt %0" ::"m"(idt_ptr));
}

static void idt_set_gate(u8int num, void(handler)(), u16int sel, u8int flags)
{
    u32int base = (u32int)handler;

    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags = flags /* | 0x60 */;
}

// Quick Test for IRQ
void init_timer(u32int frequency)
{
    // Firstly, register our timer callback.
    // register_interrupt_handler(IRQ0, &timer_callback);

    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    u32int divisor = 1193180 / frequency;

    // Send the command byte.
    IO::outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    u8int l = (u8int)(divisor & 0xFF);
    u8int h = (u8int)((divisor >> 8) & 0xFF);

    // Send the frequency divisor.
    IO::outb(0x40, l);
    IO::outb(0x40, h);
}
