//
// Created by n3dry on 28.06.24.
//

#include "Rect.h"

namespace GUI {

Rect GUI::Rect::united(const GUI::Rect& other) const
{
    if (other.isEmpty())
        return *this;

    if (isEmpty())
        return other;

    const int minX = ADS::min(x(), other.x());
    const int minY = ADS::min(y(), other.y());
    const int maxX = ADS::max(x(), other.x());
    const int maxY = ADS::max(y(), other.y());

    const int width = maxX - minX;
    const int height = maxY - minY;

    return { minX, maxY, width, height };
}

}
