//
// Created by n3dry on 17.09.22.
//

#include "Widget.h"
#include "Layout.h"
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
        if (acceptsFocus())
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
    case Event::Type::Resize:
        updateLayout();
        onResizeEvent(static_cast<ResizeEvent&>(event));
        return true;
    case Event::Type::Paint:
        for (auto& child : m_children) {
            if (child->isWidgetType()) {
                Widget* childWidget = static_cast<Widget*>(child);
                childWidget->event(event);
            }
        }
        if (isVisible())
            onPaintEvent(event);
        return true;
    case Event::Type::Show:
        for (auto& child : m_children) {
            if (child->isWidgetType()) {
                Widget* childWidget = static_cast<Widget*>(child);
                childWidget->event(event);
            }
        }
        onShowEvent(event);
        return true;
    case Event::Type::Hide:
        for (auto& child : m_children) {
            if (child->isWidgetType()) {
                Widget* childWidget = static_cast<Widget*>(child);
                childWidget->event(event);
            }
        }
        onHideEvent(event);
        return true;
    case Event::Type::Layout:
        updateLayout();
        return true;
    default:
        return CObject::event(event);
    }
}

bool Widget::hits(int x, int y, HitResult& result)
{
    for (auto& child : m_children) {
        if (!child->isWidgetType())
            continue;
        Widget* childWidget = static_cast<Widget*>(child);
        if (childWidget->hits(x - childWidget->windowRelativeRect().x(), y - childWidget->windowRelativeRect().y(), result)) {
            return true;
        }
    }

    if (rect().contains(x, y)) {
        result.widget = this;
        result.localX = x;
        result.localY = y;
        return true;
    }

    return false;
}

void Widget::updateLayout()
{
    if (!m_isVisible)
        return;

    if (m_layout)
        m_layout->activate();
}

void Widget::onShowEvent(Event& event)
{
    std::cout << "onShowEvent[" << className() << "]" << std::endl;
}

void Widget::onHideEvent(Event& event)
{
    std::cout << "onHideEvent[" << className() << "]" << std::endl;
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

void Widget::setWindowRelativeRect(const Rect& rect)
{
    if (rect == m_windowRelativeRect)
        return;

    const Size oldSize = m_windowRelativeRect.size();
    const Size newSize = rect.size();

    m_windowRelativeRect = rect;

    if (oldSize != newSize) {
        ResizeEvent resizeEvent(newSize, oldSize);
        event(resizeEvent);
    }
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
    return m_window && m_window->focusedWidget() == this && m_window->isActive();
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

void Widget::onResizeEvent(ResizeEvent& event)
{
}

void Widget::setLayout(Layout* layout)
{
    if (layout) {
        if (layout->parent() && layout->parent() != this)
            std::cout << "[Warning] Layout: has a different parent widget. Re-parenting to " << className() << std::endl;
        layout->setParent(this);
    }

    m_layout = layout;
}

Widget* Widget::parentWidget() const
{
    if (parent() && parent()->isWidgetType())
        return static_cast<Widget*>(parent());

    return nullptr;
}

Size Widget::preferredSizeHint() const
{
    if (m_layout)
        return m_layout->preferredSizeHint();

    std::cerr << "[Widget::preferredSizeHint] used for " << className() << " where no layout is provided. Probably you forgot to override preferredSizeHint()" << std::endl;
    ASSERT(false);
    return Size::Invalid();
}

Size Widget::minSizeHint() const
{
    if (m_layout)
        return m_layout->minSizeHint();

    std::cerr << "[Widget::minSizeHint] used for " << className() << " where no layout is provided. Probably you forgot to override minSizeHint()." << std::endl;
    ASSERT(false);
    return Size::Invalid();
}

} // GUI