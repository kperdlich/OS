//
// Created by n3dry on 28.05.24.
//

#include "CharacterBitmap.h"

namespace GUI {

ADS::UniquePtr<CharacterBitmap> CharacterBitmap::createFrom(IntSize size, const char* data)
{
    return ADS::UniquePtr<CharacterBitmap>(new CharacterBitmap(size, data));
}

CharacterBitmap::CharacterBitmap(IntSize size, const char* data)
    : m_size(size)
    , m_data(data)
{
}

} // GUI