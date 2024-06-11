//
// Created by n3dry on 11.06.24.
//

#include "Label.h"
#include "Painter.h"

namespace GUI {

Label::Label(Widget* parent)
    : Widget(parent)
{
}

Label::Label(const ADS::String& text, Widget* parent)
    : m_text(text)
    , Widget(parent)
{
}

void Label::onPaintEvent(Event& event)
{
    Painter painter(this);
#if 0
    painter.drawRect(m_windowRelativeRect, Colors::Black);
#endif
    painter.drawText(m_windowRelativeRect, m_text, m_alignment, Colors::Black);
}

} // GUI