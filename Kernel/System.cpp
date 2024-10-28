//
// Created by n3dry on 28.10.24.
//

#include "System.h"
#include "PIT.h"
#include "i386.h"

uint32_t getSystemUptimeMs()
{
    return g_system.ticks * PIT::TICK_DURATION_MS;
}

uint32_t getSystemUptimeSecs()
{
    return g_system.ticks / PIT::TICKS_PER_SECOND;
}

void sleep(uint32_t ms)
{
    const uint32_t end = g_system.ticks + (ms / PIT::TICK_DURATION_MS);
    while(g_system.ticks < end) {
        hlt();
    }
}