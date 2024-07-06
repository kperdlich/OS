//
// Created by n3dry on 17.09.22.
//

#include "Bitmap.h"

namespace GUI {

ADS::UniquePtr<Bitmap> Bitmap::createWrapper(Bitmap& bitmap)
{
    return ADS::UniquePtr<Bitmap>(new Bitmap(bitmap.m_format, bitmap.m_size, bitmap.m_data, OwnBitmapData::No));
}

ADS::UniquePtr<Bitmap> Bitmap::createWrapper(BitmapFormat format, Size size, char* data)
{
    ASSERT(format == BitmapFormat::RGBA32);
    return ADS::UniquePtr<Bitmap>(new Bitmap(format, size, data, OwnBitmapData::No));
}

ADS::UniquePtr<Bitmap> Bitmap::createFrom(Bitmap& bitmap)
{
    ADS::UniquePtr<Bitmap> copy = create(bitmap.m_format, bitmap.m_size);
    ADS::memcpy(copy->data(), bitmap.data(), bitmap.width() * bitmap.height() * byteDensityFor(bitmap.m_format));
    return copy;
}

ADS::UniquePtr<Bitmap> Bitmap::create(BitmapFormat format, Size size)
{
    auto* buffer = new char[size.width() * size.height() * byteDensityFor(format)];
    return ADS::UniquePtr<Bitmap>(new Bitmap(format, size, buffer, OwnBitmapData::Yes));
}

Bitmap::Bitmap(BitmapFormat format, Size size, char* data, OwnBitmapData ownership)
    : m_format(format)
    , m_size(size)
    , m_data(data)
    , m_isOwning(ownership == OwnBitmapData::Yes)
{
}

Bitmap::~Bitmap()
{
    if (m_isOwning && m_data) {
        delete m_data;
        m_data = nullptr;
    }
}

void Bitmap::setPixel(int x, int y, GUI::Color color)
{
    switch (m_format) {
    case BitmapFormat::RGBA32: {
        const int pixelByteSize = byteDensityFor(m_format);
        const int startIndex = (y * m_size.width() + x) * pixelByteSize;
        ADS::memcpy(&m_data[startIndex], &color, sizeof(GUI::Color));
        break;
    }
    default:
        ASSERT(false);
    }
}

void Bitmap::fill(GUI::Color color)
{
    const int pixelByteSize = byteDensity();
    const int pixelCount = m_size.width() * m_size.height() * pixelByteSize;
    for (int i = 0; i < pixelCount; i += pixelByteSize)
        ADS::memcpy(&m_data[i], &color, sizeof(GUI::Color));
}

int Bitmap::byteDensityFor(BitmapFormat format)
{
    switch (format) {
    case BitmapFormat::RGBA32:
        return 4;
    default:
        ASSERT(false);
    }
}

int Bitmap::byteDensity() const
{
    return byteDensityFor(m_format);
}

} // GUI