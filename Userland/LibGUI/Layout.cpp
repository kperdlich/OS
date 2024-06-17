//
// Created by n3dry on 15.06.24.
//

#include "Layout.h"
#include "Application.h"

namespace GUI {

Layout::Layout(Widget* parent)
    : CObject(parent)
{
}

void Layout::addWidget(Widget& widget)
{
    ASSERT(parent());
    ASSERT(parent()->isWidgetType());

    m_layoutItems.emplace_back(LayoutItem { .widget = &widget });
    widget.setParent(parent());

    if (parent())
        Application::instance().postEvent(parent(), ADS::UniquePtr<Event>(new Event(Event::Type::Layout)));
}

void Layout::setSpacing(int value)
{
    m_spacing = value;
    if (parent())
        Application::instance().postEvent(parent(), ADS::UniquePtr<Event>(new Event(Event::Type::Layout)));
}

} // GUI