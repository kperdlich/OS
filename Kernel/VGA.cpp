//
// Created by n3dry on 15.10.24.
//

#include "VGA.h"
#include "StdLib.h"

namespace VGA {

static constexpr const uint32_t VGA_MEMORY_ADDRESS = 0xB8000;
static constexpr const ADS::size_t VGA_WIDTH = 80;
static constexpr const ADS::size_t VGA_HEIGHT = 25;

static bool s_isInitialized { false };
static ADS::size_t s_currentRow {};
static ADS::size_t s_currentColumn {};
static uint8_t s_currentColor {};
static uint16_t* s_buffer { nullptr };

static inline uint8_t combined(Color foreground, Color background)
{
    return foreground | background << 4;
}

static inline uint16_t combined(unsigned char character, uint8_t color)
{
    return (uint16_t)character | (uint16_t)color << 8;
}

void initialize()
{
    s_currentRow = 0;
    s_currentColumn = 0;
    s_currentColor = combined(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    s_buffer = reinterpret_cast<uint16_t*>(VGA_MEMORY_ADDRESS);
    for (ADS::size_t y = 0; y < VGA_HEIGHT; y++) {
        for (ADS::size_t x = 0; x < VGA_WIDTH; x++) {
            const ADS::size_t index = y * VGA_WIDTH + x;
            s_buffer[index] = combined(' ', s_currentColor);
        }
    }
    s_isInitialized = true;
    dbgPrintf("VGA initialized\n");
}

void putChar(char c)
{
    if (c == '\n') {
        s_currentColumn = 0;
        ++s_currentRow;
        return;
    }

    putCharAt(c, s_currentColor, s_currentColumn, s_currentRow);
    if (++s_currentColumn == VGA_WIDTH) {
        s_currentColumn = 0;
        if (++s_currentRow == VGA_HEIGHT)
            s_currentRow = 0;
    }
}

void putCharAt(char c, uint8_t color, ADS::size_t x, ADS::size_t y)
{
    const ADS::size_t index = y * VGA_WIDTH + x;
    s_buffer[index] = combined(c, color);
}

void write(const char* data, ADS::size_t size)
{
    for (ADS::size_t i = 0; i < size; ++i)
        putChar(data[i]);
}

int writeString(const char* data)
{
    if (!data) {
        putChar('<');
        putChar('N');
        putChar('U');
        putChar('L');
        putChar('L');
        putChar('>');
        return 6;
    }

    const ADS::size_t len = strlen(data);
    write(data, len);
    return static_cast<int>(len);
}

void setColor(uint8_t color)
{
    s_currentColor = color;
}

bool isInitialized()
{
    return s_isInitialized;
}

}