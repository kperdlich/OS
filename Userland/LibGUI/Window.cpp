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
}

void Window::close()
{
    hide();
    GUI::WindowManager::instance().remove(*this);
    // FIXME: handle deletion
    // deleteLater();
}

void Window::event(Event& event)
{
    if (event.isMouseEvent()) {
        MouseEvent& mouseEvent = static_cast<MouseEvent&>(event);
        if (m_centralWidget) {
            Widget::HitResult result {};
            if (m_centralWidget->hits(mouseEvent.position(), result)) {
#if 0
                std::cout << "[Widget::HitResult] " << result.widget->className() << " localX: " << result.localPosition.x() << " localY: " << result.localPosition.y() << std::endl;
#endif
                MouseEvent localWidgetMouseEvent(event.type(), result.localPosition.x(), result.localPosition.y(), mouseEvent.button());
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
        if (!m_backBuffer) {
            m_backBuffer = Bitmap::create(BitmapFormat::RGBA32, size());
        }
        PaintEvent& paintEvent = static_cast<PaintEvent&>(event);
        if (m_centralWidget && m_centralWidget->windowRelativeRect().intersects(paintEvent.rect()))
            return m_centralWidget->event(event);
        return CObject::event(event);
    }

    if (event.type() == Event::Type::UpdateRequest) {
        const UpdateEvent& updateEvent = static_cast<UpdateEvent&>(event);
#if 0
        std::cout << "Window[" << title() << "] UpdateRequest from " << updateEvent.widget()->className() << " Rect: " << updateEvent.rect().toString() << std::endl;
#endif
        WindowManager::instance().invalidateWindowLocalRect(*this, updateEvent.rect());
    }

    return CObject::event(event);
}

bool Window::isWindowType() const
{
    return true;
}

bool Window::contains(Point point)
{
    return m_rect.contains(point);
}

void Window::moveBy(int x, int y)
{
    GUI::WindowManager::instance().invalidate(*this);
    m_rect.moveBy(x, y);
    GUI::WindowManager::instance().invalidate(*this);
}

void Window::resize(Size size)
{
    if (m_rect.size() == size)
        return;

    GUI::WindowManager::instance().invalidate(*this);
    m_backBuffer = nullptr;
    m_rect.setSize(size);
    GUI::WindowManager::instance().invalidate(*this);
    if (m_centralWidget)
        m_centralWidget->resize(m_rect.size());
}

void Window::resize(int width, int height)
{
    resize({ width, height });
}

void Window::setCentralWidget(Widget& widget)
{
    if (&widget == m_centralWidget)
        return;

    m_centralWidget = &widget;
    m_centralWidget->setParent(this);
    m_centralWidget->setWindow(this);

    Rect newRect = rect();
    if (m_centralWidget->verticalSizePolicy() == Widget::SizePolicy::Fixed)
        newRect.setHeight(m_centralWidget->minimumSize().height());
    else
        newRect.setHeight(m_centralWidget->preferredSizeHint().height());

    if (m_centralWidget->horizontalSizePolicy() == Widget::SizePolicy::Fixed)
        newRect.setWidth(m_centralWidget->minimumSize().width());
    else
        newRect.setWidth(m_centralWidget->preferredSizeHint().width());

    resize(newRect.size());
    m_centralWidget->resize(newRect.width(), newRect.height());
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
    WindowManager::instance().hide(*this);
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

void Window::setPosition(int x, int y)
{
    setPosition({ x, y });
}

void Window::setPosition(const Point& point)
{
    WindowManager::instance().invalidate(*this);
    m_rect.setPosition(point);
    WindowManager::instance().invalidate(*this);
}

} // GUI