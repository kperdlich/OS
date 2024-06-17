//
// Created by n3dry on 17.09.22.
//

#include "Window.h"
#include "Application.h"
#include "Event.h"
#include "Painter.h"
#include "WindowManager.h"

namespace GUI {

Window::Window(CObject* parent)
    : CObject(parent)
{
}

Window::~Window()
{
    GUI::WindowManager::instance().remove(*this);

    if (m_centralWidget) {
        delete m_centralWidget;
        m_centralWidget = nullptr;
    }
}

void Window::close()
{
    hide();
    GUI::WindowManager::instance().remove(*this);
    deleteLater();
}

bool Window::event(Event& event)
{
    if (event.isMouseEvent()) {
        MouseEvent& mouseEvent = static_cast<MouseEvent&>(event);
        if (m_centralWidget) {
            Widget::HitResult result {};
            if (m_centralWidget->hits(mouseEvent.x(), mouseEvent.y(), result)) {
#if 0
                std::cout << "[Widget::HitResult] " << result.widget->className() << " localX: " << result.localX << " localY: " << result.localY << std::endl;
#endif
                MouseEvent localWidgetMouseEvent(event.type(), result.localX, result.localY, mouseEvent.button());
                return result.widget->event(localWidgetMouseEvent);
            }
        }
        return CObject::event(event);
    }

    if (event.isKeyboardEvent()) {
        if (m_focusedWidget)
            return m_focusedWidget->event(event);
        return CObject::event(event);
    }

    if (event.isPaintEvent()) {
        if (m_centralWidget)
            return m_centralWidget->event(event);
        return CObject::event(event);
    }

    return CObject::event(event);
}

bool Window::isWindowType() const
{
    return true;
}

bool Window::contains(IntPoint point)
{
    return m_rect.contains(point);
}

void Window::moveBy(int x, int y)
{
    m_rect.moveBy(x, y);
}

void Window::setRect(const Rect& rect)
{
    Size oldSize = m_rect.size();
    m_rect = rect;
    if (m_centralWidget) {
        ResizeEvent event(m_rect.size(), oldSize);
        m_centralWidget->event(event);
    }
}

void Window::setCentralWidget(Widget& widget)
{
    if (&widget == m_centralWidget)
        return;

    if (m_centralWidget)
        m_centralWidget->setParent(nullptr);

    m_centralWidget = &widget;
    m_centralWidget->setParent(this);
    m_centralWidget->setWindow(this);

    Rect newRect = rect();
    if (m_centralWidget->verticalSizePolicy() == Widget::SizePolicy::Fixed) {
        ASSERT(!m_centralWidget->fixedSize().isInvalid());
        newRect.setHeight(m_centralWidget->fixedSize().height());
    }

    if (m_centralWidget->horizontalSizePolicy() == Widget::SizePolicy::Fixed) {
        ASSERT(!m_centralWidget->fixedSize().isInvalid());
        newRect.setWidth(m_centralWidget->fixedSize().width());
    }

    setRect(newRect);
    m_centralWidget->setWindowRelativeRect({ 0, 0, newRect.width(), newRect.height() });

    if (!m_focusedWidget && m_centralWidget->acceptsFocus())
        m_focusedWidget = m_centralWidget;
}

void Window::show()
{
    GUI::WindowManager::instance().show(*this);
    if (m_centralWidget) {
        Event showEvent(Event::Type::Show);
        m_centralWidget->event(showEvent);
    }
}

void Window::hide()
{
    if (m_centralWidget) {
        Event hideEvent(Event::Type::Hide);
        m_centralWidget->event(hideEvent);
    }
    m_isVisible = false;
}

void Window::setFocusedWidget(Widget* widget, FocusReason reason)
{
    if (m_focusedWidget == widget)
        return;

    if (m_focusedWidget)
        Application::instance().postEvent(m_focusedWidget, ADS::UniquePtr<FocusEvent>(new FocusEvent(Event::Type::FocusOut, reason)));

    m_focusedWidget = widget;

    if (m_focusedWidget)
        Application::instance().postEvent(m_focusedWidget, ADS::UniquePtr<FocusEvent>(new FocusEvent(Event::Type::FocusIn, reason)));
}

bool Window::isActive() const
{
    return WindowManager::instance().activeWindow() == this;
}

} // GUI