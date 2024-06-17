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
    painter.drawFilledRect(rect(), Colors::Black);
#endif
    painter.drawText(rect(), m_text, m_alignment, Colors::Black);
}

void Label::shrinkToFit()
{
    setHorizontalSizePolicy(Widget::SizePolicy::Fixed);
    setVerticalSizePolicy(Widget::SizePolicy::Ignore);
    setFixedSize(Size { rect().width() + (static_cast<int>(m_text.length()) * 8) + (2 * 8), 0 });
}

/*Size Label::preferredSize() const
{
    Size size { 22, 22 };
    if (m_text.empty())
        return size;

    size.setWidth(size.width() + (static_cast<int>(m_text.length()) * 8) + (2 * 8));
    return size;
}*/

} // GUI