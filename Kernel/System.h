//
// Created by n3dry on 28.10.24.
//

#pragma once

#include "Types.h"

struct System {
    ADS::time_t ticks;
};

extern System g_system;

void sleep(uint32_t ms);
uint32_t getSystemUptimeMs();
uint32_t getSystemUptimeSecs();
