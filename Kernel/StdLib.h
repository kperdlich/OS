//
// Created by n3dry on 15.10.24.
//

#pragma once

#include "Assert.h"
#include "Kprintf.h"
#include "Types.h"

ADS::size_t strlen(const char* str);
void* memset(void* ptr, uint8_t value, ADS::size_t num);

template<typename PerCharCallback>
int int32ToString(int32_t number, PerCharCallback callback);

template<typename PerCharCallback>
int uint32ToString(uint32_t value, PerCharCallback callback);

template<typename PerCharCallback>
int int32ToString(int32_t number, PerCharCallback callback)
{
    if (number < 0) {
        callback('-');
        return uint32ToString(0 - number, callback) + 1;
    }
    return uint32ToString(number, callback);
}

template<typename PerCharCallback>
int uint32ToString(uint32_t value, PerCharCallback callback)
{
    int ret = 0;
    if (value == 0) {
        callback('0');
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
        callback(digits[--index]);

    return ret;
}

template<typename PerCharCallback>
int uint32ToHexString(uint32_t value, PerCharCallback callback)
{
    static constexpr const char hexChars[] = "0123456789abcdef";
    static constexpr const uint32_t msNibble = 28; // Reading nibbles left to right (from MSB to LSB)
    static constexpr const uint32_t nibbles = 8;

    int ret = 0;
    for (ADS::size_t i = 0; i < nibbles; ++i) {
        const uint32_t shift = msNibble - (i * 4);
        const uint32_t nibble = (value >> shift) & 0xF;
        callback(hexChars[nibble]);
        ++ret;
    }

    return ret;
}