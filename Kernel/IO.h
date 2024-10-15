//
// Created by n3dry on 15.10.24.
//

#pragma once

#include "Types.h"

namespace IO {

void out8(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);

}