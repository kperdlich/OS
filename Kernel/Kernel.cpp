//
// Created by n3dry on 14.10.24.
//

#include "Assert.h"
#include "Keyboard.h"
#include "Kmalloc.h"
#include "Kprintf.h"
#include "MemoryManager.h"
#include "Multiboot.h"
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

extern "C" [[noreturn]] void kmain(uint32_t magic, multiboot_info_t* multibootInfo)
{
    cli();
    SerialDebugInterface::initialize();
    ASSERT(magic == MULTIBOOT_BOOTLOADER_MAGIC);

    VGA::initialize();
    GDT::initialize();
    IDT::initialize();
    PIT::initialize();
    Keyboard::initialize();
    MemoryManager::initialize(*multibootInfo);
    sti();

    kprintf("kprintf Test:\n");
    kprintf("Hello %s!\n", "World");
    VGA::setColor(VGA::VGA_COLOR_GREEN);
    kprintf("Hello Newline: Signed %u, Unsigned: %d\n", 12, -12);

    {
        char* ptr = reinterpret_cast<char*>(kmalloc(10000));
        ptr[0] = 'A';
        ptr[1] = 'B';
        ptr[2] = 'C';
        ptr[3] = 0;
        kprintf("Heap allocated string: %s\n", ptr);

        char* ptr2 = reinterpret_cast<char*>(kmalloc(12000));
        kfree(ptr2);
        kfree(ptr);
    }

    {
        char* ptr = reinterpret_cast<char*>(kmalloc(10000));
        kfree(ptr);
    }

    {
        char* charBuffer = new char[1337];
        delete [] charBuffer;

        uint32_t* intHeap = new uint32_t(42);
        ASSERT(*intHeap == 42);
        delete intHeap;
    }

#ifdef RUN_DIVIDE_BY_ZERO_TEST
    // Force a divide-by-zero exception
    int numerator = 1;
    int denominator = 0;
    int result = numerator / denominator; // This will trigger the divide-by-zero exception
    kprintf("Result: %d", result);
#endif

    for (;;) {
        // sleep(1000);
        // kprintf("Uptime in secs: '%u'\n", getSystemUptimeSecs());
    }
}
