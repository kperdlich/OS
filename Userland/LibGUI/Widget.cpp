//
// Created by n3dry on 17.09.22.
//

#include "Widget.h"
#include "Window.h"

namespace GUI {
Widget::Widget(Widget* parent)
    : CObject(parent)
{
}

bool Widget::event(Event& event)
{
    switch (event.type()) {
    case Event::Type::MouseDown:
        onMouseDownEvent(static_cast<MouseEvent&>(event));
        return true;
    case Event::Type::MouseUp:
        onMouseUpEvent(static_cast<MouseEvent&>(event));
        return true;
    case Event::Type::MouseMove:
        onMouseMoveEvent(static_cast<MouseEvent&>(event));
        return true;
    case Event::Type::KeyDown:
        onKeyDownEvent(static_cast<KeyEvent&>(event));
        return true;
    case Event::Type::KeyUp:
        onKeyUpEvent(static_cast<KeyEvent&>(event));
        return true;
    case Event::Type::Paint:
        onPaintEvent(event);
        return true;
    default:
        return CObject::event(event);
    }
}

bool Widget::hits(int x, int y, HitResult& result)
{
    for (auto& child : m_children) {
        Widget* childWidget = static_cast<Widget*>(child);
        if (childWidget->hits(x - childWidget->windowRelativeRect().x(), y - childWidget->windowRelativeRect().y(), result)) {
            return true;
        }
    }

    if (m_windowRelativeRect.contains(x, y)) {
        result.widget = this;
        result.localX = x;
        result.localY = y;
        return true;
    }

    return false;
}

void Widget::onPaintEvent(Event& event)
{
    for (auto& child : m_children) {
        if (child->isWidgetType()) {
            Widget* childWidget = static_cast<Widget*>(child);
            childWidget->onPaintEvent(event);
        }
    }
}

void Widget::onMouseMoveEvent(MouseEvent& event)
{
}

void Widget::onMouseDownEvent(MouseEvent& event)
{
    setFocus();
}

void Widget::onMouseUpEvent(MouseEvent& event)
{
}

void Widget::setWindowRelativeRect(const IntRect& rect)
{
    m_windowRelativeRect = rect;
}

void Widget::setWindow(Window* window)
{
    m_window = window;
    for (auto& child : m_children) {
        if (child->isWidgetType()) {
            Widget* childWidget = static_cast<Widget*>(child);
            childWidget->setWindow(window);
        }
    }
}

bool Widget::hasFocus() const
{
    ASSERT(m_window != nullptr);
    return m_window->focusedWidget() == this;
}

void Widget::setFocus()
{
    ASSERT(m_window != nullptr);
    m_window->setFocusedWidget(this);
}

bool Widget::isWidgetType() const
{
    return true;
}

void Widget::onKeyDownEvent(KeyEvent& event)
{
}

void Widget::onKeyUpEvent(KeyEvent& event)
{
}

} // GUI