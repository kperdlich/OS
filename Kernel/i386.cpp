//
// Created by n3dry on 23.10.24.
//

#include "i386.h"
#include "IO.h"
#include "StdLib.h"

namespace GDT {

static constexpr const int GDTEntries = 5;

struct GDTEntry {
    uint16_t limitLow;
    uint16_t baseLow;
    uint8_t baseMiddle;
    uint8_t access;
    uint8_t granularity;
    uint8_t baseHigh;
} __attribute__((packed));

struct GDTPointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static GDTEntry s_gdtEntries[GDTEntries];
static GDTPointer s_gdtPointer;

static void setGDTEntry(ADS::size_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    // Wtf i386 ...
    s_gdtEntries[index].baseLow = (base & 0xFFFF);
    s_gdtEntries[index].baseMiddle = (base >> 16) & 0xFF;
    s_gdtEntries[index].baseHigh = (base >> 24) & 0xFF;
    s_gdtEntries[index].limitLow = (limit & 0xFFFF);
    s_gdtEntries[index].granularity = (limit >> 16) & 0x0F;
    s_gdtEntries[index].granularity |= granularity & 0xF0;
    s_gdtEntries[index].access = access;
}

static inline void loadGDT()
{
    asm volatile("lgdt (%0)" : : "r"(&s_gdtPointer));
}

static inline void updateSegmentRegisters()
{
    asm volatile(
        "mov $0x10, %%ax \n" // 0x10 being the offset to the kernel data segment in the GDT
        "mov %%ax, %%ds \n" // Copy 0x10 into all segment registers
        "mov %%ax, %%es \n"
        "mov %%ax, %%fs \n"
        "mov %%ax, %%gs \n"
        "mov %%ax, %%ss \n"
        "jmp $0x08, $.flush \n" // Far jump to Kernel Code segment 0x08 to update the cs segment register
        ".flush: \n"
        :
        :
        : "memory");
}

void initialize()
{
    s_gdtPointer.limit = (sizeof(GDTEntry) * GDTEntries) - 1;
    s_gdtPointer.base = reinterpret_cast<uint32_t>(&s_gdtEntries);

    setGDTEntry(0, 0, 0, 0, 0); // Null segment; offset: 0x0
    setGDTEntry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code segment; offset: 0x08
    setGDTEntry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel data segment; offset: 0x10
    setGDTEntry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment; offset: 0x18
    setGDTEntry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment; offset: 0x20

    loadGDT();
    updateSegmentRegisters();
    dbgPrintf("GDT initialized\n");
}

}

namespace IDT {

struct InterruptDescriptor {
    uint16_t baseLow; // offset bits 0..15
    uint16_t selector; // a code segment selector in GDT
    uint8_t always0; // unused, set to 0
    uint8_t flags; // gate type, dpl, and p fields
    uint16_t baseHigh; // offset bits 16..31
} __attribute__((packed));

struct IDTPointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static constexpr const int IDTEntries = 256;
static InterruptDescriptor s_idtEntries[IDTEntries] {};
static IDTPointer s_idtPointer;

static inline void flushIDT()
{
    asm volatile("lidt (%0)" ::"m"(s_idtPointer));
}

#define EXCEPTION_HANDLER(i, message)          \
    static void exception_##i()                \
    {                                          \
        cli();                                 \
        dbgPrintf("Exception: " message "\n"); \
        hang();                                \
    }

EXCEPTION_HANDLER(0, "Divide Error")
EXCEPTION_HANDLER(1, "Debug")
EXCEPTION_HANDLER(2, "Non-maskable Interrupt")
EXCEPTION_HANDLER(3, "Breakpoint")
EXCEPTION_HANDLER(4, "Overflow")
EXCEPTION_HANDLER(5, "Bound Range Exceeded")
EXCEPTION_HANDLER(6, "Invalid Opcode")
EXCEPTION_HANDLER(7, "Device Not Available")
EXCEPTION_HANDLER(8, "Double Fault")
EXCEPTION_HANDLER(9, "Coprocessor Segment Overrun")
EXCEPTION_HANDLER(10, "Invalid TSS")
EXCEPTION_HANDLER(11, "Segment Not Present")
EXCEPTION_HANDLER(12, "Stack-Segment Fault")
EXCEPTION_HANDLER(13, "General Protection Fault")
EXCEPTION_HANDLER(14, "Page Fault")
EXCEPTION_HANDLER(15, "Reserved")
EXCEPTION_HANDLER(16, "x87 Floating-Point Exception")

static void setIDTEntry(ADS::size_t index, uint32_t handlerAddress, uint16_t selector, uint8_t flags)
{
    s_idtEntries[index].baseLow = handlerAddress & 0xFFFF;
    s_idtEntries[index].selector = selector;
    s_idtEntries[index].always0 = 0;
    s_idtEntries[index].flags = flags;
    s_idtEntries[index].baseHigh = (handlerAddress >> 16) & 0xFFFF;
}

static void initializeExceptions()
{
    setIDTEntry(0, reinterpret_cast<uint32_t>(exception_0), 0x08, 0x8E);
    setIDTEntry(1, reinterpret_cast<uint32_t>(exception_1), 0x08, 0x8E);
    setIDTEntry(2, reinterpret_cast<uint32_t>(exception_2), 0x08, 0x8E);
    setIDTEntry(3, reinterpret_cast<uint32_t>(exception_3), 0x08, 0x8E);
    setIDTEntry(4, reinterpret_cast<uint32_t>(exception_4), 0x08, 0x8E);
    setIDTEntry(5, reinterpret_cast<uint32_t>(exception_5), 0x08, 0x8E);
    setIDTEntry(6, reinterpret_cast<uint32_t>(exception_6), 0x08, 0x8E);
    setIDTEntry(7, reinterpret_cast<uint32_t>(exception_7), 0x08, 0x8E);
    setIDTEntry(8, reinterpret_cast<uint32_t>(exception_8), 0x08, 0x8E);
    setIDTEntry(9, reinterpret_cast<uint32_t>(exception_9), 0x08, 0x8E);
    setIDTEntry(10, reinterpret_cast<uint32_t>(exception_10), 0x08, 0x8E);
    setIDTEntry(11, reinterpret_cast<uint32_t>(exception_11), 0x08, 0x8E);
    setIDTEntry(12, reinterpret_cast<uint32_t>(exception_12), 0x08, 0x8E);
    setIDTEntry(13, reinterpret_cast<uint32_t>(exception_13), 0x08, 0x8E);
    setIDTEntry(14, reinterpret_cast<uint32_t>(exception_14), 0x08, 0x8E);
    setIDTEntry(15, reinterpret_cast<uint32_t>(exception_15), 0x08, 0x8E);
    setIDTEntry(16, reinterpret_cast<uint32_t>(exception_16), 0x08, 0x8E);
}

static const constexpr uint16_t IRQ_Count = 16;
static const constexpr uint32_t IRQ_Base = 0x20;
static const constexpr uint32_t PIC_MasterCmd = 0x20;
static const constexpr uint32_t PIC_MasterData = 0x21;
static const constexpr uint32_t PIC_SlaveCmd = 0xA0;
static const constexpr uint32_t PIC_SlaveData = 0xA1;
static const constexpr uint32_t PIC_EOI = 0x020;

static IRQHandler s_irqHandler[IRQ_Count] { nullptr };

static void sendEOIToPIC(uint8_t irq)
{
    if (irq >= 8)
        IO::out8(PIC_SlaveCmd, PIC_EOI);
    IO::out8(PIC_MasterCmd, PIC_EOI);
}

static uint16_t readISRFromPIC()
{
    IO::out8(PIC_MasterCmd, 0x0b);
    IO::out8(PIC_SlaveCmd, 0x0b);
    return (IO::inb8(PIC_SlaveCmd) << 8) | IO::inb8(PIC_MasterCmd);
}

static void remapPIC()
{
    // ICW1
    IO::out8(PIC_MasterCmd, 0x11); // Start initialization of Master
    IO::out8(PIC_SlaveCmd, 0x11); // Start initialization of Slave

    // ICW2: Remap offset
    IO::out8(PIC_MasterData, IRQ_Base); // Remap PIC1 to 0x20–0x27
    IO::out8(PIC_SlaveData, IRQ_Base + 0x08); // Remap PIC2 to 0x28–0x2F

    // ICW3: Cascading
    IO::out8(PIC_MasterData, 0x04);
    IO::out8(PIC_SlaveData, 0x02);

    // ICW4: Enable 8086 mode
    IO::out8(PIC_MasterData, 0x01);
    IO::out8(PIC_SlaveData, 0x01);

    // Disable all IRQs
    IO::out8(PIC_MasterData, 0xFF);
    IO::out8(PIC_SlaveData, 0xFF);

    // IRQ2 must be enabled to allow interrupts from the Slave PIC to be passed through to the CPU.
    enableIRQ(2);
}

extern "C" void handleIRQ();
extern "C" void commonIRQEntry();
asm(
    ".globl commonIRQEntry\n"
    "commonIRQEntry: \n"
    "pusha\n"
    "call handleIRQ\n"
    "popa\n"
    "iret\n");

void handleIRQ()
{
    const uint16_t isr = readISRFromPIC();
    if (isr == 0) {
        dbgPrintf("Spurious IRQ\n");
        return;
    }

    int32_t irq = -1;
    for (uint16_t i = 0; i < IRQ_Count; ++i) {
        if (isr & (1 << i)) {
            irq = i;
            break;
        }
    }
    ASSERT(irq >= 0 && irq < IRQ_Count);

    if (s_irqHandler[irq])
        s_irqHandler[irq]();
    else
        dbgPrintf("No IRQ handler found for irq: %u\n", irq);

    sendEOIToPIC(irq);
}

static void initializeIRQs()
{
    for (ADS::size_t i = 0; i < IRQ_Count; ++i)
        setIDTEntry(IRQ_Base + i, reinterpret_cast<uint32_t>(commonIRQEntry), 0x08, 0x8E);
}

void initialize()
{
    s_idtPointer.limit = (sizeof(InterruptDescriptor) * IDTEntries) - 1;
    s_idtPointer.base = reinterpret_cast<uint32_t>(&s_idtEntries);

    initializeExceptions();
    remapPIC();
    initializeIRQs();

    flushIDT();
    dbgPrintf("IDT initialized\n");
}

void enableIRQ(uint8_t irq)
{
    uint16_t port;
    if (irq < 8) {
        port = PIC_MasterData;
    } else {
        port = PIC_SlaveData;
        irq -= 8;
    }

    const uint8_t value = IO::inb8(port) & ~(1 << irq); // Clear bit to enable the IRQ
    IO::out8(port, value);
}

void disableIRQ(uint8_t irq)
{
    uint16_t port;
    if (irq < 8) {
        port = PIC_MasterData;
    } else {
        port = PIC_SlaveData;
        irq -= 8;
    }

    const uint8_t value = IO::inb8(port) | (1 << irq); // Set bit to disable the IRQ
    IO::out8(port, value);
}

void registerIRQHandler(uint8_t irq, IRQHandler handler)
{
    ASSERT(irq >= 0 && irq < IRQ_Count);
    s_irqHandler[irq] = handler;
}

}
