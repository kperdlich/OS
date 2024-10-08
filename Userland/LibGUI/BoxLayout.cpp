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
    const Size maxAvailableSize = owner.size();
    Size availableSize = maxAvailableSize;
    int visibleItems = 0;
    int fixedItems = 0;
    for (auto& item : m_layoutItems) {
        if (!item.widget || !item.widget->isVisible())
            continue;
        Widget& widget = *item.widget;
        ++visibleItems;
        const bool hasFixedSize = (m_direction == Direction::Vertical && widget.verticalSizePolicy() == Widget::SizePolicy::Fixed)
            || (m_direction == Direction::Horizontal && widget.horizontalSizePolicy() == Widget::SizePolicy::Fixed);
        if (hasFixedSize) {
            availableSize -= widget.minimumSize();
            ++fixedItems;
        }
        availableSize -= Size { spacing(), spacing() };
    }
    // No spacing at the end
    availableSize += Size { spacing(), spacing() };

    const int automaticSizeItems = visibleItems - fixedItems;
    const Size automaticItemSize = automaticSizeItems > 0 ? Size { availableSize.width() / automaticSizeItems, availableSize.height() / automaticSizeItems } : Size {};

#if 1
    std::cout << "[BoxLayout::activate()] automaticSizeItems: " << automaticSizeItems << " automaticItemSize: " << automaticItemSize.toString() <<  " Spacing: " << spacing() << std::endl;
#endif

    int currentX = 0;
    int currentY = 0;

    for (auto& item : m_layoutItems) {
        if (!item.widget || !item.widget->isVisible())
            continue;

        Widget& widget = *item.widget;
        Rect rect { currentX, currentY, 0, 0 };

        // FIXME: Add margin
        if (m_direction == Direction::Vertical) {
            if (widget.horizontalSizePolicy() == Widget::SizePolicy::Fixed)
                rect.setWidth(widget.minimumSize().width());
            else
                rect.setWidth(maxAvailableSize.width());

            if (widget.verticalSizePolicy() == Widget::SizePolicy::Fixed)
                rect.setHeight(widget.minimumSize().height());
            else
                rect.setHeight(automaticItemSize.height());

            currentY += rect.height() + spacing();
        } else {
            if (widget.horizontalSizePolicy() == Widget::SizePolicy::Fixed)
                rect.setWidth(widget.minimumSize().width());
            else
                rect.setWidth(automaticItemSize.width());

            if (widget.verticalSizePolicy() == Widget::SizePolicy::Fixed)
                rect.setHeight(widget.minimumSize().height());
            else
                rect.setHeight(maxAvailableSize.height());

            currentX += rect.width() + spacing();
        }
        item.widget->setRelativeRect(rect);
    }
}

Size BoxLayout::preferredSizeHint() const
{
    Size preferredSize {};
    for (auto& item : m_layoutItems) {
        if (!item.widget || !item.widget->isWidgetType())
            continue;

        int width { 0 };
        if (item.widget->horizontalSizePolicy() == Widget::SizePolicy::Fixed)
            width = item.widget->minimumSize().width();
        else
            width = item.widget->preferredSizeHint().width();

        int height { 0 };
        if (item.widget->verticalSizePolicy() == Widget::SizePolicy::Fixed)
            height = item.widget->minimumSize().height();
        else
            height = item.widget->preferredSizeHint().height();

        preferredSize += { width, height };
        preferredSize += { spacing(), spacing() };
    }
    preferredSize -= { spacing(), spacing() };
    return preferredSize;
}

Size BoxLayout::minSizeHint() const
{
    Size minSize {};
    for (auto& items : m_layoutItems) {
        if (!items.widget || !items.widget->isWidgetType())
            continue;
        minSize += items.widget->minSizeHint();
        minSize += { spacing(), spacing() };
    }
    minSize -= { spacing(), spacing() };
    return minSize;
}

} // GUI