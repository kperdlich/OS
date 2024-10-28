//
// Created by n3dry on 23.10.24.
//

#pragma once

#include "Types.h"

using IRQHandler = void (*)();

constexpr const uint32_t IRQ_Timer = 0;
constexpr const uint32_t IRQ_Keyboard = 1;

#define LSB(x) ((x) & 0xFF)
#define MSB(x) (((x) >> 8) & 0xFF)
#define hang() asm volatile("cli; hlt")
#define hlt() asm volatile("hlt")
#define cli() asm volatile("cli")
#define sti() asm volatile("sti")

namespace GDT {
void initialize();
}

namespace IDT {
void initialize();
void registerIRQHandler(uint8_t irq, IRQHandler handler);
void enableIRQ(uint8_t irq);
void disableIRQ(uint8_t irq);
}
