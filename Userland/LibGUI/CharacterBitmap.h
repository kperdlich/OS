//
// Created by n3dry on 28.05.24.
//

#pragma once

#include "Size.h"
#include "Types.h"

namespace GUI {

class CharacterBitmap final {
public:
    CharacterBitmap(IntSize size, const char* data);

    inline const char* data() const { return m_data; }
    inline IntSize size() const { return m_size; }
    inline int width() const { return m_size.width(); }
    inline int height() const { return m_size.height(); }

private:
    IntSize m_size;
    const char* m_data { nullptr };
};

} // GUI
