//
// Created by n3dry on 14.10.24.
//

#include "SerialDebug.h"
#include "Types.h"
#include "Kprintf.h"
#include "VGA.h"
#include "Assert.h"

#ifdef __linux__
    #error "Kernel needs to be compiled with a cross-compiler"
#endif

#ifndef __i386__
    #error "Kernel needs to be compiled with an x86-elf compiler"
#endif

#ifndef KERNEL
    #error "Kernel is compiled but KERNEL is not defined"
#endif

extern "C" void kmain()
{
    SerialDebugInterface::initialize();
    VGA::initialize();

    kprintf("kprintf Test:\n");
    kprintf("Hello %s!\n", "World");
    VGA::setColor(VGA::VGA_COLOR_GREEN);
    kprintf("Hello Newline: Signed %u, Unsigned: %d", 12, -12);
}
