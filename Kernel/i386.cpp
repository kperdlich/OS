//
// Created by n3dry on 23.10.24.
//

#include "i386.h"
#include "StdLib.h"

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

static void setGDTGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    // Wtf i386 ...
    s_gdtEntries[num].baseLow = (base & 0xFFFF);
    s_gdtEntries[num].baseMiddle = (base >> 16) & 0xFF;
    s_gdtEntries[num].baseHigh = (base >> 24) & 0xFF;
    s_gdtEntries[num].limitLow = (limit & 0xFFFF);
    s_gdtEntries[num].granularity = (limit >> 16) & 0x0F;
    s_gdtEntries[num].granularity |= granularity & 0xF0;
    s_gdtEntries[num].access = access;
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

void initializeGDT()
{
    s_gdtPointer.limit = (sizeof(GDTEntry) * GDTEntries) - 1;
    s_gdtPointer.base = reinterpret_cast<uint32_t>(&s_gdtEntries);

    setGDTGate(0, 0, 0, 0, 0); // Null segment; offset: 0x0
    setGDTGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code segment; offset: 0x08
    setGDTGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel data segment; offset: 0x10
    setGDTGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment; offset: 0x18
    setGDTGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment; offset: 0x20

    loadGDT();
    updateSegmentRegisters();
    dbgPrintf("GDT initialized\n");
}

void disableInterrupts()
{
    asm volatile("cli");
}

void enableInterrupts()
{
    asm volatile("sti");
}
