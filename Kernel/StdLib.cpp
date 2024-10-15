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