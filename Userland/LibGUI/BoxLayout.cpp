//
// Created by n3dry on 15.06.24.
//

#include "BoxLayout.h"
#include "Widget.h"

namespace GUI {

BoxLayout::BoxLayout(BoxLayout::Direction direction, Widget* parent)
    : m_direction(direction)
    , Layout(parent)
{
}

void BoxLayout::activate()
{
#if 0
    std::cout << "[BoxLayout::activate]" << std::endl;
#endif

    Widget& owner = *parentWidget();
    const Size maxAvailableSize = owner.rect().size();
    Size availableSize = maxAvailableSize;
    size_t visibleItems = 0;
    for (auto& item : m_layoutItems) {
        if (!item.widget->isVisible())
            continue;

        ++visibleItems;
        availableSize -= Size { spacing(), spacing() };
    }

    const int items = static_cast<int>(m_layoutItems.size());
    const Size sizePerItem = { availableSize.width() / items, availableSize.height() / items };

    int currentX = owner.rect().x();
    int currentY = owner.rect().y();

    for (auto& item : m_layoutItems) {
        if (!item.widget->isVisible())
            continue;

        Rect rect {currentX, currentY, 0, 0};

        // FIXME: Add margin
        if (m_direction == Direction::Vertical) {
            rect.setWidth(maxAvailableSize.width());
            rect.setHeight(sizePerItem.height());
            currentY += rect.height() + spacing();
        } else {
            rect.setHeight(maxAvailableSize.height());
            rect.setWidth(sizePerItem.width());
            currentX += rect.width() + spacing();
        }
        item.widget->setWindowRelativeRect(rect);
    }
}

} // GUI