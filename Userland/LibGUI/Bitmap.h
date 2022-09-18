//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "Color.h"
#include "Rect.h"

namespace GUI {

enum class BitmapFormat {
    RGBA32
};

class Bitmap final {
public:
    Bitmap() = default;
    static Bitmap* createFrom(BitmapFormat format, Rect size, char* data);

    inline char* data() { return m_data; }
    inline Rect size() { return m_size; }

    void setPixel(int x, int y, GUI::Color color);
    void clear(GUI::Color color);

private:
    Bitmap(BitmapFormat format, Rect size, char* data);

private:
    Rect m_size;
    BitmapFormat m_format;
    char* m_data { nullptr };
};

} // GUI
