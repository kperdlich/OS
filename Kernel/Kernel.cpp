//
// Created by n3dry on 14.10.24.
//

#include "Assert.h"
#include "Keyboard.h"
#include "Kprintf.h"
#include "PIT.h"
#include "SerialDebug.h"
#include "System.h"
#include "Types.h"
#include "VGA.h"
#include "i386.h"

#ifdef __linux__
#error "Kernel needs to be compiled with a cross-compiler"
#endif

#ifndef __i386__
#error "Kernel needs to be compiled with an x86-elf compiler"
#endif

#ifndef KERNEL
#error "Kernel is compiled but KERNEL is not defined"
#endif

System g_system {};

extern "C" [[noreturn]] void kmain()
{
    cli();
    SerialDebugInterface::initialize();
    VGA::initialize();
    GDT::initialize();
    IDT::initialize();
    PIT::initialize();
    Keyboard::initialize();
    sti();

    kprintf("kprintf Test:\n");
    kprintf("Hello %s!\n", "World");
    VGA::setColor(VGA::VGA_COLOR_GREEN);
    kprintf("Hello Newline: Signed %u, Unsigned: %d\n", 12, -12);

#ifdef RUN_DIVIDE_BY_ZERO_TEST
    // Force a divide-by-zero exception
    int numerator = 1;
    int denominator = 0;
    int result = numerator / denominator; // This will trigger the divide-by-zero exception
    kprintf("Result: %d", result);
#endif

    for (;;) {
        //sleep(1000);
        //kprintf("Uptime in secs: '%u'\n", getSystemUptimeSecs());
    }
}
