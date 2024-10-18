//
// Created by n3dry on 18.10.24.
//

#pragma once

#include "SerialDebug.h"

#define ASSERT(x)                                                                                                           \
    do {                                                                                                                    \
        if (!(x)) {                                                                                                         \
            asm volatile("cli");                                                                                            \
            SerialDebugInterface::printf("ASSERTION Failed: '" #x "':\n%s:%u %s", __FILE__, __LINE__, __PRETTY_FUNCTION__); \
            asm volatile("hlt");                                                                                            \
        }                                                                                                                   \
    } while (false)

#define ASSERT_NOT_REACHED() ASSERT(false)
