//
// Created by n3dry on 15.10.24.
//

#include "SerialDebug.h"
#include "IO.h"
#include "StdLib.h"

namespace SerialDebugInterface {

static bool s_isInitialized { false };
static constexpr const uint32_t COM1 = 0x3F8;

static inline int isTransmitEmpty()
{
    return IO::inb8(COM1 + 5) & 0x20;
}

void putChar(char a)
{
    while (isTransmitEmpty() == 0)
        ;

    IO::out8(COM1, a);
}

int write(const char* str)
{
    if (!str) {
        putChar('<');
        putChar('N');
        putChar('U');
        putChar('L');
        putChar('L');
        putChar('>');
        return 6;
    }

    const ADS::size_t length = strlen(str);
    for (ADS::size_t i = 0; i < length; ++i)
        putChar(str[i]);
    return length;
}

void initialize()
{
    IO::out8(COM1 + 1, 0x00); // Disable interrupts
    IO::out8(COM1 + 3, 0x80); // Enable DLAB (set baud rate divisor)
    IO::out8(COM1 + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    IO::out8(COM1 + 1, 0x00); // (hi byte)
    IO::out8(COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
    IO::out8(COM1 + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    IO::out8(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set

    s_isInitialized = true;
    write("Serial Debug initialized\n");
}

bool isInitialized()
{
    return s_isInitialized;
}

}
