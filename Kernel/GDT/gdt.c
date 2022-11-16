#include "gdt.h"
#include "../vga.h"

static void Hang()
{
    asm volatile("cli; hlt");
}

typedef struct registers {
    u32int gs, fs, es, ds;
    u32int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    u32int int_no, err_code; // Interrupt number and error code (if applicable)
    u32int eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

#define NAKED __attribute__((naked))

/*
#define EXCEPTION_HANDLER(idx, msg)       \
    static void exception_handler_##idx() \
    {                                     \
        terminal_writestring(msg);        \
        Hang();                           \
    }
*/

#define ISR_HANDLER(idx, name)             \
    void name##_isr_handler(registers_t);  \
    NAKED void name##_isr_asm_entry()      \
    {                                      \
        asm(                               \
            "cli\n"                        \
            "pushl $0x0\n"                 \
            "pushl $" #idx "\n"            \
            "pusha\n"                      \
            "mov $0x10, %ax\n"             \
            "mov %ax, %ds\n"               \
            "mov %ax, %es\n"               \
            "mov %ax, %fs\n"               \
            "mov %ax, %gs\n"               \
            "call " #name "_isr_handler\n" \
            "mov %ds, %ax\n"               \
            "mov %es, %ax\n"               \
            "mov %fs, %ax\n"               \
            "mov %gs, %ax\n"               \
            "popa\n"                       \
            "addl $0x8, %esp\n"            \
            "sti\n"                        \
            "iret\n");                     \
    }

#define ISR_HANDLER_WITH_ERROR_CODE(idx, name) \
    void name##_isr_handler(registers_t);      \
    NAKED void name##_isr_asm_entry()          \
    {                                          \
        asm(                                   \
            "cli\n"                            \
            "pushl $" #idx "\n"                \
            "pusha\n"                          \
            "mov $0x10, %ax\n"                 \
            "mov %ax, %ds\n"                   \
            "mov %ax, %es\n"                   \
            "mov %ax, %fs\n"                   \
            "mov %ax, %gs\n"                   \
            "call " #name "_isr_handler\n"     \
            "mov %ds, %ax\n"                   \
            "mov %es, %ax\n"                   \
            "mov %fs, %ax\n"                   \
            "mov %gs, %ax\n"                   \
            "popa\n"                           \
            "addl $0x4, %esp\n"                \
            "sti\n"                            \
            "iret\n");                         \
    }
/*
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
 */

ISR_HANDLER(0, division_by_zero)
void division_by_zero_isr_handler(registers_t reg)
{
    terminal_writestring("division_by_zero_isr_handler");
}

ISR_HANDLER(1, debug_exception)
void debug_exception_isr_handler(registers_t reg)
{
    terminal_writestring("debug_exception_isr_handler");
}

ISR_HANDLER(2, non_maskable_interrupt)
void non_maskable_interrupt_isr_handler(registers_t reg)
{
    terminal_writestring("non_maskable_interrupt_isr_handler");
}

ISR_HANDLER(3, breakpoint)
void breakpoint_isr_handler(registers_t reg)
{
    terminal_writestring("breakpoint_isr_handler");
}

ISR_HANDLER(4, detected_overflow)
void detected_overflow_isr_handler(registers_t reg)
{
    terminal_writestring("detected_overflow_isr_handler");
}

ISR_HANDLER(5, out_of_bounds)
void out_of_bounds_isr_handler(registers_t reg)
{
    terminal_writestring("out_of_bounds_isr_handler");
}

ISR_HANDLER(6, invalid_opcode)
void invalid_opcode_isr_handler(registers_t reg)
{
    terminal_writestring("invalid_opcode_isr_handler");
}

ISR_HANDLER(7, no_coprocessor)
void no_coprocessor_isr_handler(registers_t reg)
{
    terminal_writestring("no_coprocessor_isr_handler");
}

ISR_HANDLER(9, coprocessor_segment_overrun)
void coprocessor_segment_overrun_isr_handler(registers_t reg)
{
    terminal_writestring("coprocessor_segment_overrun_isr_handler");
}

ISR_HANDLER(15, unknown_interrupt_exception)
void unknown_interrupt_exception_isr_handler(registers_t reg)
{
    terminal_writestring("unknown_interrupt_exception_isr_handler");
}

ISR_HANDLER(16, coprocessor_fault)
void coprocessor_fault_isr_handler(registers_t reg)
{
    terminal_writestring("coprocessor_fault_isr_handler");
}

ISR_HANDLER(17, alignment_check_exception)
void alignment_check_exception_isr_handler(registers_t reg)
{
    terminal_writestring("alignment_check_exception_isr_handler");
}

ISR_HANDLER(18, machine_check_exception)
void machine_check_exception_isr_handler(registers_t reg)
{
    terminal_writestring("machine_check_exception_isr_handler");
}

ISR_HANDLER_WITH_ERROR_CODE(8, double_fault)
void double_fault_isr_handler(registers_t reg)
{
    terminal_writestring("double_fault_isr_handler");
}

ISR_HANDLER_WITH_ERROR_CODE(10, bad_tss)
void bad_tss_isr_handler(registers_t reg)
{
    terminal_writestring("bad_tss_fault_isr_handler");
}

ISR_HANDLER_WITH_ERROR_CODE(11, segment_not_found)
void segment_not_found_isr_handler(registers_t reg)
{
    terminal_writestring("segment_not_fault");
}

ISR_HANDLER_WITH_ERROR_CODE(12, stack_fault)
void stack_fault_isr_handler(registers_t reg)
{
    terminal_writestring("stack_fault_isr_handler");
}

ISR_HANDLER_WITH_ERROR_CODE(13, general_protection_fault)
void general_protection_fault_isr_handler(registers_t reg)
{
    if (reg.int_no == 13) {
        terminal_writestring("general_protection_fault_isr_handler");
    }
}

ISR_HANDLER_WITH_ERROR_CODE(14, page_fault)
void page_fault_isr_handler(registers_t reg)
{
    terminal_writestring("page_fault_isr_handler");
}

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

// Internal function prototypes.
static void init_gdt();
static void gdt_set_gate(s32int, u32int, u32int, u8int, u8int);

static void init_idt();
static void idt_set_gate(u8int, u32int, u16int, u8int);

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

    idt_set_gate(0, division_by_zero_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(1, debug_exception_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(2, non_maskable_interrupt_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(3, breakpoint_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(4, detected_overflow_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(5, out_of_bounds_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(6, invalid_opcode_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(7, no_coprocessor_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(8, double_fault_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(9, coprocessor_segment_overrun_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(10, bad_tss_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(11, segment_not_found_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(12, stack_fault_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(13, general_protection_fault_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(14, page_fault_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(15, unknown_interrupt_exception_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(16, coprocessor_segment_overrun_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(17, alignment_check_exception_isr_asm_entry, 0x08, 0x8E);
    idt_set_gate(18, machine_check_exception_isr_asm_entry, 0x08, 0x8E);

    asm volatile("lidt %0" ::"m"(idt_ptr));
}

static void idt_set_gate(u8int num, u32int base, u16int sel, u8int flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags = flags /* | 0x60 */;
}