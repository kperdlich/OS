//
// Created by n3dry on 15.10.24.
//

#include "IO.h"

namespace IO {

void out8(uint16_t port, uint8_t value)
{
    asm("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

}
