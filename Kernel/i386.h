//
// Created by n3dry on 23.10.24.
//

#pragma once

#include "Types.h"

#define hang() asm volatile("cli; hlt")
#define cli() asm volatile("cli")
#define sti() asm volatile("sti")

void initializeGDT();
void initializeIDT();
void enableIRQ(uint8_t irq);
void disableIRQ(uint8_t irq);
