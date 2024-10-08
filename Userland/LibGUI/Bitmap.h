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

    static ADS::OwnPtr<Bitmap> createWrapper(BitmapFormat format, Size size, char* data);
    static ADS::OwnPtr<Bitmap> createWrapper(Bitmap& bitmap);
    static ADS::OwnPtr<Bitmap> createFrom(Bitmap& bitmap);
    static ADS::OwnPtr<Bitmap> create(BitmapFormat format, Size size);

    static int byteDensityFor(BitmapFormat format);

    char* data() { return m_data; }
    [[nodiscard]] Size size() const { return m_size; }
    [[nodiscard]] int width() const { return m_size.width(); }
    [[nodiscard]] int height() const { return m_size.height(); }
    [[nodiscard]] BitmapFormat format() const { return m_format; }

    [[nodiscard]] int byteDensity() const;
    void setPixel(int x, int y, GUI::Color color);
    void fill(GUI::Color color);

private:
    enum class OwnBitmapData {
        Yes,
        No
    };

    Bitmap(BitmapFormat format, Size size, char* data, size_t dataSize, OwnBitmapData ownership);

private:
    Size m_size;
    size_t m_dataSize {};
    char* m_data { nullptr };
    BitmapFormat m_format;
    bool m_isOwning { false };
};

} // GUI
