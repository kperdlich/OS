//
// Created by n3dry on 17.09.22.
//

#include "Bitmap.h"
#include "Types.h"

namespace GUI {

Bitmap::Bitmap(BitmapFormat format, IntSize size, char* data)
    : m_format(format)
    , m_size(size)
    , m_data(data)
{
}

Bitmap* Bitmap::createFrom(BitmapFormat format, IntSize size, char* data)
{
    auto bitmap = new Bitmap(format, size, data);
    return bitmap;
}

void Bitmap::setPixel(int x, int y, GUI::Color color)
{
    switch (m_format) {
    case BitmapFormat::RGBA32: {
        const int pixelByteSize = 4;
        const int startIndex = (y * m_size.width() + x) * pixelByteSize;
        ADS::memcpy(&m_data[startIndex], &color, sizeof(GUI::Color));
        break;
    }
    default:
        ASSERT(false);
    }
}
void Bitmap::clear(GUI::Color color)
{
    switch (m_format) {
    case BitmapFormat::RGBA32: {
        const int pixelByteSize = 4;
        const int pixelCount = m_size.width() * m_size.height() * pixelByteSize;
        for (int i = 0; i < pixelCount; i += pixelByteSize) {
            ADS::memcpy(&m_data[i], &color, sizeof(GUI::Color));
        }
        break;
    }
    default:
        ASSERT(false);
    }
}
} // GUI