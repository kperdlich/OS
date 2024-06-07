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
        setFocus(FocusReason::Mouse);
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
    case Event::Type::FocusIn:
        onFocusInEvent(static_cast<FocusEvent&>(event));
        return true;
    case Event::Type::FocusOut:
        onFocusOutEvent(static_cast<FocusEvent&>(event));
        return true;
    case Event::Type::Paint:
        for (auto& child : m_children) {
            if (child->isWidgetType()) {
                Widget* childWidget = static_cast<Widget*>(child);
                childWidget->onPaintEvent(event);
            }
        }
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
}

void Widget::onMouseMoveEvent(MouseEvent& event)
{
}

void Widget::onMouseDownEvent(MouseEvent& event)
{
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
    return m_window->focusedWidget() == this && m_window->isActive();
}

void Widget::setFocus(FocusReason reason)
{
    ASSERT(m_window != nullptr);
    m_window->setFocusedWidget(this, reason);
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

void Widget::onFocusOutEvent(FocusEvent& event)
{
}

void Widget::onFocusInEvent(FocusEvent& event)
{
}

} // GUI