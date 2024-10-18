//
// Created by n3dry on 15.10.24.
//

#include "SerialDebug.h"
#include "Assert.h"
#include "IO.h"
#include "StdLib.h"
#include <stdarg.h>

namespace SerialDebugInterface {

static constexpr const uint32_t COM1 = 0x3F8;

static inline int isTransmitEmpty()
{
    return IO::inb(COM1 + 5) & 0x20;
}

static void putChar(char a)
{
    while (isTransmitEmpty() == 0)
        ;

    IO::out8(COM1, a);
}

static int write(const char* str)
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

static int writeUnsigned(uint32_t value)
{
    int ret = 0;
    if (value == 0) {
        putChar('0');
        return ++ret;
    }

    // maximum for 32-bit int is 10 digits
    char digits[10] { 0 };
    int index = 0;

    // Convert
    while (value > 0) {
        digits[index++] = '0' + (value % 10);
        value /= 10;
        ++ret;
    }

    // Conversion is in reverse order
    while (index > 0)
        putChar(digits[--index]);

    return ret;
}

static int writeSigned(int32_t number)
{
    if (number < 0) {
        putChar('-');
        return writeUnsigned(0 - number) + 1;
    }
    return writeUnsigned(number);
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

    printf("Serial Debug initialized\n");
}

int printf(const char* format, ...)
{
    int ret = 0;
    va_list args;
    va_start(args, format);

    // FIXME: make this reusable
    for (; *format; ++format) {
        if (*format == '%' && *(format + 1)) {
            ++format;
            switch (*format) {
            case 's':
                ret += write(va_arg(args, const char*));
                break;
            case 'd':
                ret += writeSigned(va_arg(args, int32_t));
                break;
            case 'u':
                ret += writeUnsigned(va_arg(args, uint32_t));
                break;
            default:
                ASSERT_NOT_REACHED();
            }
        } else {
            putChar(*format);
            ++ret;
        }
    }

    va_end(args);
    return ret;
}

}
