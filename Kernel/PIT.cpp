//
// Created by n3dry on 28.10.24.
//

#include "PIT.h"
#include "IO.h"
#include "i386.h"
#include "System.h"
#include "Kprintf.h"

namespace PIT {

void timerHandler()
{
    g_system.ticks++;
}

void initialize()
{
    IO::out8(COMMAND_PORT, COMMAND);
    const uint16_t divisor = FREQUENCY / TICKS_PER_SECOND;
    IO::out8(CHANNEL_0_PORT, LSB(divisor));
    IO::out8(CHANNEL_0_PORT, MSB(divisor));

    IDT::registerIRQHandler(IRQ_Timer, timerHandler);
    IDT::enableIRQ(IRQ_Timer);
    dbgPrintf("PIT initialized\n");
}

}