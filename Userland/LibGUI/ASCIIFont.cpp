//
// Created by n3dry on 14.05.24.
//

#include "ASCIIFont.h"

namespace GUI {
ASCIIFont::ASCIIFont(const char* const* font, char firstChar, char lastChar, size_t width, size_t height)
    : m_asciiChars(font)
    , m_firstChar(firstChar)
    , m_lastChar(lastChar)
    , m_height(height)
    , m_width(width)
{
}

const char* ASCIIFont::GetAsciiBitmapForChar(char asciiChar)
{
    if (asciiChar < m_firstChar || asciiChar > m_lastChar)
        return nullptr;

    const size_t index = asciiChar - m_firstChar;
    return m_asciiChars[index];
}

} // GUI