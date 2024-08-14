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

int Label::fontWidth()
{
    // FIXME: get this from the font
    return 8;
}

void Label::onPaintEvent(PaintEvent& event)
{
    Painter painter(*this);
    painter.setClipRect(event.rect());
#if 0
    painter.drawFilledRect(rect(), Colors::Black);
#endif
    painter.drawText(rect(), m_text, m_alignment, Colors::Black);
}

void Label::shrinkToFit()
{
    setHorizontalSizePolicy(Widget::SizePolicy::Fixed);
    setVerticalSizePolicy(Widget::SizePolicy::Automatic);
    setFixedSize(Size { rect().width() + (static_cast<int>(m_text.length()) * fontWidth()) + (2 * 5), 0 });
    update();
}

Size Label::preferredSizeHint() const
{
    if (m_text.isEmpty())
        return minSizeHint();

    Size size = minSizeHint();
    size.setWidth(ADS::max((static_cast<int>(m_text.length()) * fontWidth()) + (2 * 5), size.width()));
    return size;
}

Size Label::minSizeHint() const
{
    return { 22, 22 };
}

} // GUI