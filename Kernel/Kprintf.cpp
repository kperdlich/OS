//
// Created by n3dry on 23.10.24.
//

#include "Kprintf.h"
#include "Assert.h"
#include "SerialDebug.h"
#include "StdLib.h"
#include "VGA.h"
#include <stdarg.h>

template<typename PerCharCallback>
inline int forEachCharIn(const char* str, PerCharCallback callback)
{
    const ADS::size_t len = strlen(str);
    for (ADS::size_t i = 0; i < len; ++i)
        callback(str[i]);
    return static_cast<int>(len);
}

template<typename PerCharCallback>
inline int printfTemplate(PerCharCallback callback, const char*& format, va_list& args)
{
    int characters = 0;
    for (; *format; ++format) {
        if (*format == '%' && *(format + 1)) {
            ++format;
            switch (*format) {
            case 's':
                characters += forEachCharIn(va_arg(args, const char*), callback);
                break;
            case 'c':
                callback(static_cast<char>(va_arg(args, int)));
                ++characters;
                break;
            case 'd':
                characters += int32ToString(va_arg(args, int32_t), callback);
                break;
            case 'u':
                characters += uint32ToString(va_arg(args, uint32_t), callback);
                break;
            case 'x':
                characters += uint32ToHexString(va_arg(args, uint32_t), callback);
                break;
            case 'p':
                callback('0');
                callback('x');
                characters += 2;
                characters += uint32ToHexString(va_arg(args, uint32_t), callback);
                break;
            default:
                ASSERT_NOT_REACHED();
            }
        } else {
            callback(*format);
            ++characters;
        }
    }

    return characters;
}

int kprintf(const char* format, ...)
{
    ASSERT(VGA::isInitialized());

    va_list args;
    va_start(args, format);
    const int ret = printfTemplate(VGA::putChar, format, args);
    va_end(args);
    return ret;
}

int dbgPrintf(const char* format, ...)
{
    ASSERT(SerialDebugInterface::isInitialized());

    va_list args;
    va_start(args, format);
    const int ret = printfTemplate(SerialDebugInterface::putChar, format, args);
    va_end(args);
    return ret;
}
