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
#if 1
    std::cout << "[BoxLayout::activate] " << std::endl;
#endif

    ASSERT(parent()->isWidgetType());
    Widget& owner = static_cast<Widget&>(*parent());
    const Size maxAvailableSize = owner.rect().size();

    Size availableSize = maxAvailableSize;
    int visibleItems = 0;
    int fixedItems = 0;
    int preferredItems = 0;
    for (auto& item : m_layoutItems) {
        if (!item.widget || !item.widget->isVisible())
            continue;
        Widget& widget = *item.widget;
        ++visibleItems;

        const bool isWidgetWithFixedSize = (m_direction == Direction::Vertical && widget.verticalSizePolicy() == Widget::SizePolicy::Fixed)
            || (m_direction == Direction::Horizontal && widget.horizontalSizePolicy() == Widget::SizePolicy::Fixed);
        if (isWidgetWithFixedSize) {
            availableSize -= widget.fixedSize();
            ++fixedItems;
        }

        availableSize -= Size { spacing(), spacing() };
    }
    // No spacing at the end
    availableSize += Size { spacing(), spacing() };

    const int automaticItems = visibleItems - preferredItems - fixedItems;
    const Size automaticSize = automaticItems > 0 ? Size { availableSize.width() / automaticItems, availableSize.height() / automaticItems } : Size {};

    int currentX = owner.rect().x();
    int currentY = owner.rect().y();

    for (auto& item : m_layoutItems) {
        if (!item.widget || !item.widget->isVisible())
            continue;

        Widget& widget = *item.widget;
        Rect rect { currentX, currentY, 0, 0 };

        // FIXME: Add margin
        if (m_direction == Direction::Vertical) {
            if (widget.horizontalSizePolicy() == Widget::SizePolicy::Fixed)
                rect.setWidth(widget.fixedSize().width());
            else
                rect.setWidth(maxAvailableSize.width());

            if (widget.verticalSizePolicy() == Widget::SizePolicy::Fixed)
                rect.setHeight(widget.fixedSize().height());
            else
                rect.setHeight(automaticSize.height());

            currentY += rect.height() + spacing();
        } else {
            if (widget.horizontalSizePolicy() == Widget::SizePolicy::Fixed)
                rect.setWidth(widget.fixedSize().width());
            else
                rect.setWidth(automaticSize.width());

            if (widget.verticalSizePolicy() == Widget::SizePolicy::Fixed)
                rect.setHeight(widget.fixedSize().height());
            else
                rect.setHeight(maxAvailableSize.height());

            currentX += rect.width() + spacing();
        }
        item.widget->setWindowRelativeRect(rect);
    }
}

} // GUI