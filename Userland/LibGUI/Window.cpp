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
    if (m_centralWidget) {
        delete m_centralWidget;
        m_centralWidget = nullptr;
    }

    GUI::WindowManager::instance().remove(*this);
}

bool Window::event(Event& event)
{
    if (event.isMouseEvent()) {
        MouseEvent& mouseEvent = static_cast<MouseEvent&>(event);
        if (m_centralWidget) {
            Widget::HitResult result {};
            if (m_centralWidget->hits(mouseEvent.x(), mouseEvent.y(), result)) {
                //std::cout << "[Widget::HitResult] " << result.widget->name() << " localX: " << result.localX << " localY: " << result.localY << std::endl;
                return result.widget->event(event);
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
    // FIXME: Handle resize event?
    m_rect = rect;
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

    if (!m_focusedWidget)
        m_focusedWidget = m_centralWidget;
}

void Window::show()
{
    GUI::WindowManager::instance().add(*this);
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