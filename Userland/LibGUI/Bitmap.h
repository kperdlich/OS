//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "Color.h"
#include "Size.h"
#include "Types.h"

namespace GUI {

enum class BitmapFormat {
    RGBA32
};

class Bitmap final {
public:
    Bitmap() = default;
    static ADS::UniquePtr<Bitmap> createFrom(BitmapFormat format, IntSize size, char* data);

    inline char* data() { return m_data; }
    inline IntSize size() { return m_size; }

    void setPixel(int x, int y, GUI::Color color);
    void fill(GUI::Color color);

private:
    Bitmap(BitmapFormat format, IntSize size, char* data);

private:
    IntSize m_size;
    BitmapFormat m_format;
    char* m_data { nullptr };
};

} // GUI
