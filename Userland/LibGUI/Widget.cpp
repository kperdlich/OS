//
// Created by n3dry on 17.09.22.
//

#include "Widget.h"
#include "Layout.h"
#include "Window.h"
#include "WindowManager.h"

namespace GUI {
Widget::Widget(Widget* parent)
    : CObject(parent)
{
}

void Widget::event(Event& event)
{
    switch (event.type()) {
    case Event::Type::MouseDown:
        if (acceptsFocus())
            setFocus(FocusReason::Mouse);
        return onMouseDownEvent(static_cast<MouseEvent&>(event));
    case Event::Type::MouseUp:
        return onMouseUpEvent(static_cast<MouseEvent&>(event));
    case Event::Type::MouseMove:
        return onMouseMoveEvent(static_cast<MouseEvent&>(event));
    case Event::Type::KeyDown:
        return onKeyDownEvent(static_cast<KeyEvent&>(event));
    case Event::Type::KeyUp:
        return onKeyUpEvent(static_cast<KeyEvent&>(event));
    case Event::Type::FocusIn:
        return onFocusInEvent(static_cast<FocusEvent&>(event));
    case Event::Type::FocusOut:
        return onFocusOutEvent(static_cast<FocusEvent&>(event));
    case Event::Type::Resize:
        updateLayout();
        return onResizeEvent(static_cast<ResizeEvent&>(event));
    case Event::Type::Paint: {
        PaintEvent& paintEvent = static_cast<PaintEvent&>(event);
        Painter painter(*this);
        painter.setClipRect(paintEvent.rect());
        painter.drawFilledRect(rect(), Colors::Grey);
#if 0
        std::cout << "[Widget::paint] " << className() << " rect: " << relativeRect().toString() << std::endl;
#endif
        if (isVisible())
            onPaintEvent(paintEvent);
        for (auto& child : m_children) {
            if (!child->isWidgetType())
                continue;
            Widget* childWidget = static_cast<Widget*>(child);
            if (!childWidget->isVisible())
                continue;
            if (!childWidget->relativeRect().intersects(paintEvent.rect()))
                continue;
            Rect localIntersectionArea = paintEvent.rect().intersectRect(childWidget->relativeRect());
            localIntersectionArea.moveBy(-childWidget->relativePosition());
            PaintEvent localPaintEvent(localIntersectionArea);
            childWidget->event(localPaintEvent);
        }
        return;
    }
    case Event::Type::Show:
        for (auto& child : m_children) {
            if (child->isWidgetType()) {
                Widget* childWidget = static_cast<Widget*>(child);
                childWidget->event(event);
            }
        }
        onShowEvent(event);
        return;
    case Event::Type::Hide:
        for (auto& child : m_children) {
            if (child->isWidgetType()) {
                Widget* childWidget = static_cast<Widget*>(child);
                childWidget->event(event);
            }
        }
        onHideEvent(event);
        return;
    case Event::Type::Layout:
        updateLayout();
        return;
    default:
        return CObject::event(event);
    }
}

bool Widget::hits(const Point& point, HitResult& result)
{
    for (auto& child : m_children) {
        if (!child->isWidgetType())
            continue;
        Widget* childWidget = static_cast<Widget*>(child);
        if (!childWidget->isVisible())
            continue;
        if (childWidget->hits(point - childWidget->relativePosition(), result)) {
            return true;
        }
    }

    if (rect().contains(point)) {
        result.widget = this;
        result.localPosition = point;
        return true;
    }

    return false;
}

void Widget::updateLayout()
{
    if (!m_isVisible)
        return;

    if (m_layout) {
        m_layout->activate();
        update();
    }
}

void Widget::onShowEvent(Event& event)
{
    std::cout << "onShowEvent[" << className() << "]" << std::endl;
}

void Widget::onHideEvent(Event& event)
{
    std::cout << "onHideEvent[" << className() << "]" << std::endl;
}

void Widget::onPaintEvent(PaintEvent& event)
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

void Widget::setRelativeRect(const Rect& rect)
{
    if (rect == m_relativeRect)
        return;

    const Size oldSize = m_relativeRect.size();
    const Size newSize = rect.size();

    m_relativeRect = rect;

    if (oldSize != newSize) {
        ResizeEvent resizeEvent(newSize, oldSize);
        event(resizeEvent);
    }

    update();
}

bool Widget::hasFocus() const
{
    return window() && window()->focusedWidget() == this && window()->isActive();
}

void Widget::setFocus(FocusReason reason)
{
    ASSERT(window() != nullptr);
    window()->setFocusedWidget(this, reason);
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
    updateLayout();
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
    // ASSERT(false);
    return Size::Invalid();
}

Size Widget::minSizeHint() const
{
    if (m_layout)
        return m_layout->minSizeHint();

    std::cerr << "[Widget::minSizeHint] used for " << className() << " where no layout is provided. Probably you forgot to override minSizeHint()." << std::endl;
    // ASSERT(false);
    return Size::Invalid();
}

void Widget::grabMouse()
{
    WindowManager::instance().setMouseGrabbedWidget(*this);
}

void Widget::releaseMouse()
{
    WindowManager::instance().releaseMouseGrabbedWidget();
}

void Widget::resize(int width, int height)
{
    resize({ width, height });
}

void Widget::resize(const Size& newSize)
{
    setRelativeRect({ relativePosition(), newSize });
}

void Widget::setFixedSize(const Size& value)
{
    m_minimumSize = value;
    updateLayout();
    update();
}

Rect Widget::windowRelativeRect() const
{
    Rect currentRect = m_relativeRect;
    for (Widget* parent = parentWidget(); parent; parent = parent->parentWidget())
        currentRect.moveBy(parent->relativePosition());
    return currentRect;
}

void Widget::moveTo(int x, int y)
{
    m_relativeRect.setY(y);
    m_relativeRect.setX(x);
    update();
}

void Widget::setIsVisible(bool value)
{
    m_isVisible = value;
    update();
}

void Widget::update()
{
    update(rect());
}

void Widget::update(const Rect& rect)
{
    if (Window* widgetWindow = window()) {
        Rect widgetRelativeRect = rect;
        widgetRelativeRect.moveBy(windowRelativeRect().position());
        UpdateEvent event(*this, widgetRelativeRect);
        widgetWindow->event(event);
    }
}

} // GUI