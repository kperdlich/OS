//
// Created by n3dry on 15.10.24.
//

#include "StdLib.h"

ADS::size_t strlen(const char* str)
{
    ADS::size_t len = 0;
    while (str[len] != 0)
        len++;
    return len;
}

void* memset(void* ptr, uint8_t value, ADS::size_t num)
{
    for (ADS::size_t i = 0; i < num; ++i)
        static_cast<uint8_t*>(ptr)[i] = value;
    return ptr;
}
