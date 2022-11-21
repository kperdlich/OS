#include "gdt.h"
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


gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

// Internal function prototypes.
static void init_gdt();
static void gdt_set_gate(s32int, u32int, u32int, u8int, u8int);

static void init_idt();
static void idt_set_gate(u8int, void (handler)(), u16int, u8int);

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
    idt_set_gate(12, exception_handler_12, 0x08, 0x8E);
    idt_set_gate(13, exception_handler_13, 0x08, 0x8E);
    idt_set_gate(14, exception_handler_14, 0x08, 0x8E);
    idt_set_gate(15, exception_handler_15, 0x08, 0x8E);
    idt_set_gate(16, exception_handler_16, 0x08, 0x8E);
    idt_set_gate(17, exception_handler_17, 0x08, 0x8E);
    idt_set_gate(18, exception_handler_18, 0x08, 0x8E);

    for (int i = 19; i <= 31; ++i) {
        idt_set_gate(i, exception_handler_19_31, 0x08, 0x8E);
    }

    asm volatile("lidt %0" ::"m"(idt_ptr));
}

static void idt_set_gate(u8int num, void (handler)(), u16int sel, u8int flags)
{
    u32int base = (u32int) handler;

    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags = flags /* | 0x60 */;
}
