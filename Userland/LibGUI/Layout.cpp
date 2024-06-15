//
// Created by n3dry on 15.06.24.
//

#include "Layout.h"
#include "Application.h"

namespace GUI {

Layout::Layout(Widget* parent)
    : Widget(parent)
{
}

void Layout::addWidget(Widget& widget)
{
    m_layoutItems.emplace_back(LayoutItem { .widget = &widget });
    if (Widget* parent = parentWidget())
        Application::instance().postEvent(parent, ADS::UniquePtr<Event>(new Event(Event::Type::Layout)));
}

void Layout::setSpacing(int value)
{
    m_spacing = value;
    if (Widget* parent = parentWidget())
        Application::instance().postEvent(parent, ADS::UniquePtr<Event>(new Event(Event::Type::Layout)));
}

} // GUI