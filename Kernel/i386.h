//
// Created by n3dry on 23.10.24.
//

#pragma once

#define HANG() asm volatile("cli; hlt")
#define disableInterrupts() asm volatile("cli")
#define enableInterrupts() asm volatile("sti")

void initializeGDT();
void initializeIDT();
