//
// Created by n3dry on 28.10.24.
//

#pragma once

#include "Types.h"

namespace PIT {

static constexpr const uint32_t FREQUENCY = 1193182;
static constexpr const uint8_t TICKS_PER_SECOND = 100;
static constexpr const uint32_t TICK_DURATION_MS = (1000 / TICKS_PER_SECOND);
static constexpr const uint8_t COMMAND_PORT = 0x43;
static constexpr const uint8_t CHANNEL_0_PORT = 0x40;
static constexpr const uint8_t CHANNEL_0 = 0x00;
static constexpr const uint8_t ACCESS_MODE_2BYTE = 0x30;
static constexpr const uint8_t MODE_SQUARE_WAVE = 0x06;
static constexpr const uint8_t DISABLED_BCD = 0x00;
static constexpr const uint8_t COMMAND = (CHANNEL_0 | ACCESS_MODE_2BYTE | MODE_SQUARE_WAVE | DISABLED_BCD);

void initialize();

}
