//
// Created by n3dry on 14.10.24.
//

#include "SerialDebug.h"
#include "Types.h"
#include "VGA.h"

#ifdef __linux__
    #error "Kernel needs to be compiled with an cross-compiler"
#endif

#ifndef __i386__
    #error "Kernel needs to be compiled with an x86-elf compiler"
#endif

#ifndef KERNEL
    #error "Kernel is compiled but KERNEL is not defined"
#endif

extern "C" void kmain()
{
    SerialDebug::initialize();

    VGA::initialize();
    VGA::writeString("Hello, kernel World!\n");
}
