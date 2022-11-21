////////////////////////////////////////////////////////////////////////////
//
// -------------------------------------------------------------------------
//  File name:   string.h
//  Version:     v1.00
//  Created:     13/4/2014 by Kevin.
//  Description: define Methods to work with string
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "types.h"

void ConvertInt2String(sInt32_t value, char* valuestring);

size_t strlen(const char* str);

void* memset(void* ptr, int value, size_t num);