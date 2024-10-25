//
// Created by n3dry on 23.10.24.
//

#pragma once

#define hang() asm volatile("cli; hlt")
#define cli() asm volatile("cli")
#define sti() asm volatile("sti")

void initializeGDT();
void initializeIDT();
