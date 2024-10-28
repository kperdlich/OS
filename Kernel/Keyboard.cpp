//
// Created by n3dry on 28.10.24.
//

#include "Keyboard.h"
#include "IO.h"
#include "Kprintf.h"
#include "i386.h"

namespace Keyboard {

static constexpr const uint32_t KEY_RELEASED_OFFSET = 0x80;
static constexpr const uint32_t LEFT_SHIFT_PRESSED = 0x2A;
static constexpr const uint32_t LEFT_SHIFT_RELEASED = LEFT_SHIFT_PRESSED + KEY_RELEASED_OFFSET;

static constexpr const uint32_t DATA_PORT = 0x60;
static constexpr const uint32_t STATUS_PORT = 0x64;
static constexpr const uint32_t STATUS_FLAG_DATA_AVAILABLE = 0x01;

static const char s_scanCodes[] = {
    0,
    0, // ESC
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
    '\b', // Backspace
    '\t', // Tab
    'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', '[', ']',
    '\n', // Enter
    0, // Left Control
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, // Left Shift
    '\\', 'y', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0, // Right Shift
    '*',
    0, // Left Alt
    ' ',
    0, // Caps Lock
};

static const char s_scanCodesShifted[] = {
    0,
    0, // ESC
    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
    '\b', // Backspace
    '\t', // Tab
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
    '\n', // Enter
    0, // Left Control
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, // Left Shift
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
    0, // Right Shift
    '*',
    0, // Left Alt
    ' ',
    0, // Caps Lock
};

enum KeyboardModifier {
    NoModifier = 0x00,
    ShiftModifier = 0x01,
    AltModifier = 0x02,
    CtrlModifier = 0x04,
};

static uint8_t s_modifier = NoModifier;

void keyboardHandler()
{
    while (IO::inb8(STATUS_PORT) & STATUS_FLAG_DATA_AVAILABLE) {
        const uint8_t scanCode = IO::inb8(DATA_PORT);
        switch (scanCode) {
        case LEFT_SHIFT_PRESSED:
            s_modifier |= ShiftModifier;
            break;
        case LEFT_SHIFT_RELEASED:
            s_modifier &= ~ShiftModifier;
            break;
        default:
            if (scanCode & KEY_RELEASED_OFFSET) {
                // key has been depressed
                break;
            }

            if (scanCode >= sizeof(s_scanCodes)) {
                dbgPrintf("Couldn't get key for scancode: %u\n", scanCode);
                break;
            }

            uint8_t key = s_scanCodes[scanCode];
            if (s_modifier & ShiftModifier)
                key = s_scanCodesShifted[scanCode];
            kprintf("%c", key);
            break;
        }
    }
}

void initialize()
{
    // Clear data buffer
    while (IO::inb8(STATUS_PORT) & STATUS_FLAG_DATA_AVAILABLE)
        IO::inb8(DATA_PORT);

    IDT::registerIRQHandler(IRQ_Keyboard, keyboardHandler);
    IDT::enableIRQ(IRQ_Keyboard);
    dbgPrintf("PS/2 Keyboard initialized\n");
}
}
