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

    Rect rect;
    rect.setLeft(ADS::min(left(), other.left()));
    rect.setTop(ADS::min(top(), other.top()));
    rect.setRight(ADS::max(right(), other.right()));
    rect.setBottom(ADS::max(bottom(), other.bottom()));
    return rect;
}

}
