//
// Created by n3dry on 15.10.24.
//

#pragma once

#include "Types.h"

namespace SerialDebugInterface {

void initialize();
bool isInitialized();
void putChar(char a);
int write(const char* str);

}
