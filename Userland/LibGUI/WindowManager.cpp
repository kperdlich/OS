//
// Created by n3dry on 21.09.22.
//

#include "WindowManager.h"
#include "Application.h"
#include "CharacterBitmap.h"
#include "Painter.h"
#include "Screen.h"
#include "Widget.h"
#include <chrono>

namespace GUI {

static constexpr const int TitleBarHeight = 20;
static constexpr const int TitleBarButtonsMargin = 2;
static constexpr const int TitleBarButtonSize = TitleBarHeight - (2 * TitleBarButtonsMargin);
static const GUI::Color InactiveTitleBarColor = Colors::DarkGrey;
static const GUI::Color InactiveTitleBarTextColor = Colors::LightWhite;
static const GUI::Color ActiveWindowTitleBarColor = Colors::DarkBlue;
static const GUI::Color ActiveWindowTitleBarTextColor = Colors::White;

// FIXME get this from framebuffer
static constexpr const int width = 1024;
static constexpr const int height = 720;

static const Size closeButtonCharSize { 11, 9 };
static constexpr const char* closeButtonCharacters {
    " ##     ## "
    "  ##   ##  "
    "   ## ##   "
    "    ###    "
    "     #     "
    "    ###    "
    "   ## ##   "
    "  ##   ##  "
    " ##     ## "
};

static const CharacterBitmap closeButtonBitmap(closeButtonCharSize, closeButtonCharacters);

static constexpr const int FrameWidthMargin = 1;
static constexpr const int TaskbarHeight = 20;
static const GUI::Color TaskbarColor { 190, 190, 190, 0xff };
static const Rect TaskbarRect { 0, height - TaskbarHeight, width, TaskbarHeight };

static Rect windowFrameRect(const Window& window)
{
    const auto& rect = window.rect();
    return { rect.x() - FrameWidthMargin, rect.y() - TitleBarHeight, rect.width() + (2 * FrameWidthMargin), rect.height() + TitleBarHeight + 1 };
}

static Rect windowTitleBarCloseButtonRect(const Window& window)
{
    const auto& rect = window.rect();
    return { rect.x() + rect.width() + FrameWidthMargin - TitleBarButtonSize - TitleBarButtonsMargin, rect.y() - TitleBarHeight + TitleBarButtonsMargin, TitleBarButtonSize, TitleBarButtonSize };
}

static Rect windowTitleBarRect(const Window& window)
{
    const auto& rect = window.rect();
    return { rect.x() - FrameWidthMargin, rect.y() - TitleBarHeight, rect.width() + (2 * FrameWidthMargin), TitleBarHeight };
}

static void paintTaskbar()
{
    Painter painter;
    painter.drawFilledRect(TaskbarRect, TaskbarColor);
}

bool WindowManager::event(Event& event)
{
    if (event.isMouseEvent()) {
        processMouseEvent(static_cast<MouseEvent&>(event));
        return true;
    }

    if (event.isKeyboardEvent()) {
        if (m_activeWindow) {
            return m_activeWindow->event(event);
        }
        return CObject::event(event);
    }

    return CObject::event(event);
}

void WindowManager::show(Window& window)
{
    m_windows.emplace_back(&window);
    if (window.isVisible())
        repaint(window);
}

void WindowManager::remove(Window& window)
{
    // FIXME: This sucks
    for (auto iter = m_windows.begin(); iter != m_windows.end();) {
        if (*iter == &window) {
            m_windows.erase(iter++);
            break;
        } else {
            ++iter;
        }
    }
}

void WindowManager::makeActive(Window* window)
{
    if (m_activeWindow == window)
        return;

    // FIXME: Handle z-sorting better
    if (window) {
        remove(*window);
        show(*window);
    }

    if (m_activeWindow && m_activeWindow->focusedWidget())
        Application::instance().postEvent(m_activeWindow->focusedWidget(), ADS::UniquePtr<FocusEvent>(new FocusEvent(Event::Type::FocusOut, FocusReason::ActiveWindow)));

    Window* const previousActiveWindow = m_activeWindow;
    m_activeWindow = window;

    if (previousActiveWindow) {
        if (previousActiveWindow->isVisible()) {
            repaint(*previousActiveWindow);
        } else {
            hide(*previousActiveWindow);
        }
    }

    if (m_activeWindow) {
        repaint(*m_activeWindow);
    }

    if (m_activeWindow && m_activeWindow->focusedWidget())
        Application::instance().postEvent(m_activeWindow->focusedWidget(), ADS::UniquePtr<FocusEvent>(new FocusEvent(Event::Type::FocusIn, FocusReason::ActiveWindow)));
}

void WindowManager::processMouseEvent(MouseEvent& event)
{
    if (m_mouseGrabbedWidget && m_mouseGrabbedWidget->isVisible()) {
        ASSERT(m_mouseGrabbedWidget->window());
        const Rect widgetWindowRelativePosition = m_mouseGrabbedWidget->windowRelativeRect();
        const Rect windowPosition = m_mouseGrabbedWidget->window()->rect();
        MouseEvent localWidgetMouseEvent(event.type(), event.x() - windowPosition.x() - widgetWindowRelativePosition.x(), event.y() - windowPosition.y() - widgetWindowRelativePosition.y(), event.button());
        m_mouseGrabbedWidget->event(localWidgetMouseEvent);
        return;
    }

    if (event.type() == Event::Type::MouseMove && m_isDraggingWindow) {
        const Rect oldRectWindowRect = windowFrameRect(*m_activeWindow);
        Point pos = m_dragWindowOrigin;
        pos.moveBy(event.x() - m_dragOrigin.x(), event.y() - m_dragOrigin.y());
        m_activeWindow->setPosition(pos);
        Painter painter;
        std::cout << "Hide oldRectWindowRect rect: " << oldRectWindowRect.toString() << std::endl;
        painter.drawFilledRect(oldRectWindowRect, Colors::White);
        repaintOverlappingWindow(windowFrameRect(*m_activeWindow));
        return;
    }

    if (event.type() == Event::Type::MouseUp && m_isDraggingWindow) {
        m_isDraggingWindow = false;
        return;
    }

    const IteratorResult result = forEachVisibleWindowFrontToBack([&](Window& window) -> IteratorResult {
        if (event.type() == Event::Type::MouseDown) {
            if (windowTitleBarRect(window).contains(event.position())) {
                if (windowTitleBarCloseButtonRect(window).contains(event.position())) {
                    closeWindow(window);
                    return IteratorResult::Break;
                }

                makeActive(&window);
                onWindowTaskBarMouseDown(window, event.x(), event.y());
                return IteratorResult::Break;
            }
        }

        if (window.contains(event.position())) {
            if (event.type() == Event::Type::MouseDown) {
                makeActive(&window);
            }
            MouseEvent mouseEventRelativeToWindow(event.type(), event.x() - window.rect().x(), event.y() - window.rect().y());
            window.event(mouseEventRelativeToWindow);
            return IteratorResult::Break;
        }

        return IteratorResult::Continue;
    });

    const bool didHitNoWindow = event.type() == Event::Type::MouseDown && result != IteratorResult::Break;
    if (didHitNoWindow)
        makeActive(nullptr);
}

void WindowManager::paintWindowFrame(Window& window)
{
    const bool isActiveWindow = m_activeWindow == &window;

    Painter painter;
    painter.drawFilledRect(windowTitleBarRect(window), isActiveWindow ? ActiveWindowTitleBarColor : InactiveTitleBarColor);

    Rect closeButtonRect = windowTitleBarCloseButtonRect(window);
    IntPoint closeButtonBitmapPos = closeButtonRect.position();
    closeButtonBitmapPos.moveBy(3, 3);
    painter.drawFilledRect(closeButtonRect, Colors::Grey);
    painter.drawCharacterBitmap(closeButtonBitmapPos, closeButtonBitmap, Colors::Black);

    painter.drawRect(windowTitleBarCloseButtonRect(window), Colors::Black);
    painter.drawText(windowTitleBarRect(window), window.title(), Alignment::Center, isActiveWindow ? ActiveWindowTitleBarTextColor : InactiveTitleBarTextColor);
    painter.drawRect(windowFrameRect(window), isActiveWindow ? Colors::Black : InactiveTitleBarColor);
}

void WindowManager::onWindowTaskBarMouseDown(Window& window, int x, int y)
{
    const auto& rect = window.rect();
    if (!windowTitleBarCloseButtonRect(window).contains(x, y)) {
        m_dragOrigin = { x, y };
        m_dragWindowOrigin = window.rect().position();
        m_isDraggingWindow = true;
    }
}

void WindowManager::closeWindow(Window& window)
{
    makeActive(nullptr);
    window.close();
}

void WindowManager::releaseMouseGrabbedWidget()
{
    m_mouseGrabbedWidget = nullptr;
}

void WindowManager::setMouseGrabbedWidget(Widget& widget)
{
    m_mouseGrabbedWidget = &widget;
}

void WindowManager::invalidateWindowRect(Window& window, const Rect& rect)
{
    Application::instance().postEvent(&window, ADS::UniquePtr<PaintEvent>(new PaintEvent(rect)));
}

void WindowManager::hide(Window& window)
{
    Painter painter;
    painter.drawFilledRect(windowFrameRect(window), Colors::White);
}

void WindowManager::repaint(Window& window)
{
    std::cout << "[WindowManager::repaint] " << window.title() << " rect: " << windowFrameRect(window).toString() << std::endl;
    paintWindowFrame(window);
    Rect localWindowRect = window.rect();
    localWindowRect.moveBy(-window.rect().position());
    paintWindowFrame(window);
    PaintEvent event(localWindowRect);
    window.event(event);
}

void WindowManager::repaintOverlappingWindow(const Rect& rect)
{
    forEachVisibleWindowBackToFront([&](Window& window) -> IteratorResult {
        if (rect.intersects(windowFrameRect(window))) {
            repaint(window);
        }
        return IteratorResult::Continue;
    });
}

} // GUI