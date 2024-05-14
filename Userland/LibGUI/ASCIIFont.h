//
// Created by n3dry on 14.05.24.
//

#pragma once

#include "Array.h"
#include "Bitmap.h"
#include "Types.h"

namespace GUI {

class ASCIIFont final {
public:
    ASCIIFont(const char* const* font, char firstChar, char lastChar, size_t width, size_t height);

    const char* GetAsciiBitmapForChar(char asciiChar);

    inline size_t width() { return m_width; }
    inline size_t height() { return m_height; };

private:
    char m_firstChar {};
    char m_lastChar {};
    size_t m_width {};
    size_t m_height {};
    const char* const* m_asciiChars {};
};

} // GUI
