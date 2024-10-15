//
// Created by n3dry on 15.10.24.
//

#include "VGA.h"
#include "StdLib.h"

namespace VGA {

static constexpr const uint32_t VGA_MEMORY_ADDRESS = 0xB8000;
static constexpr const ADS::size_t VGA_WIDTH = 80;
static constexpr const ADS::size_t VGA_HEIGHT = 25;

static ADS::size_t currentRow;
static ADS::size_t currentColumn;
static uint8_t currentColor;
static uint16_t* buffer;

static inline uint8_t combined(Color foreground, Color background)
{
    return foreground | background << 4;
}

static inline uint16_t combined(unsigned char character, uint8_t color)
{
    return (uint16_t) character | (uint16_t) color << 8;
}

void initialize()
{
    currentRow = 0;
    currentColumn = 0;
    currentColor = combined(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    buffer = reinterpret_cast<uint16_t*>(VGA_MEMORY_ADDRESS);
    for (ADS::size_t y = 0; y < VGA_HEIGHT; y++) {
        for (ADS::size_t x = 0; x < VGA_WIDTH; x++) {
            const ADS::size_t index = y * VGA_WIDTH + x;
            buffer[index] = combined(' ', currentColor);
        }
    }
}

void putChar(char c)
{
    putCharAt(c, currentColor, currentColumn, currentRow);
    if (++currentColumn == VGA_WIDTH) {
        currentColumn = 0;
        if (++currentRow == VGA_HEIGHT)
            currentRow = 0;
    }
}

void putCharAt(char c, uint8_t color, ADS::size_t x, ADS::size_t y)
{
    const ADS::size_t index = y * VGA_WIDTH + x;
    buffer[index] = combined(c, color);
}

void write(const char* data, ADS::size_t size)
{
    for (ADS::size_t i = 0; i < size; ++i)
        putChar(data[i]);
}

void writeString(const char* data)
{
    write(data, strlen(data));
}

void setColor(uint8_t color)
{
    currentColor = color;
}

}