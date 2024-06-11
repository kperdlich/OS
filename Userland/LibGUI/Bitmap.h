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
    ~Bitmap();

    static ADS::UniquePtr<Bitmap> createFrom(BitmapFormat format, Size size, char* data);

    inline char* data() { return m_data; }
    inline Size size() const { return m_size; }
    inline int width() const { return m_size.width(); }
    inline int height() const { return m_size.height(); }

    void setPixel(int x, int y, GUI::Color color);
    void fill(GUI::Color color);

private:
    Bitmap(BitmapFormat format, Size size, char* data);

private:
    Size m_size;
    BitmapFormat m_format;
    char* m_data { nullptr };
};

} // GUI
