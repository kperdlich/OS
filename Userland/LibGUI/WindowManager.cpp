//
// Created by n3dry on 21.09.22.
//

#include "WindowManager.h"
#include "CharacterBitmap.h"
#include "Painter.h"
#include "Screen.h"
#include "Widget.h"

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

static const IntSize closeButtonCharSize { 11, 9 };
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

static constexpr const int TaskbarHeight = 20;
static const GUI::Color TaskbarColor { 190, 190, 190, 0xff };
static const IntRect TaskbarRect { 0, height - TaskbarHeight, width, TaskbarHeight };

static IntRect windowFrameRect(const Window& window)
{
    const auto& rect = window.rect();
    return { rect.x(), rect.y() - TitleBarHeight, rect.width(), rect.height() + TitleBarHeight };
}

static IntRect windowTitleBarCloseButtonRect(const Window& window)
{
    const auto& rect = window.rect();
    return { rect.x() + rect.width() - TitleBarButtonSize - TitleBarButtonsMargin, rect.y() - TitleBarHeight + TitleBarButtonsMargin, TitleBarButtonSize, TitleBarButtonSize };
}

static IntRect windowTitleBarRect(const Window& window)
{
    const auto& rect = window.rect();
    return { rect.x(), rect.y() - TitleBarHeight, rect.width(), TitleBarHeight };
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

    if (event.isPaintEvent()) {
        processPaintEvent(event);
        return true;
    }

    return CObject::event(event);
}

void WindowManager::add(Window& window)
{
    m_windows.emplace_back(&window);
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

void WindowManager::makeActive(Window& window)
{
    // FIXME: Handle z-sorting better
    remove(window);
    add(window);

    m_activeWindow = &window;
}

void WindowManager::processMouseEvent(MouseEvent& event)
{
    if (event.type() == Event::Type::MouseMove && m_isDraggingWindow) {
        const int deltaX = event.x() - m_lastMouseDragPos.width();
        const int deltaY = event.y() - m_lastMouseDragPos.height();
        m_activeWindow->moveBy(deltaX, deltaY);
        m_lastMouseDragPos = { event.x(), event.y() };
        return;
    }

    if (event.type() == Event::Type::MouseUp && m_isDraggingWindow) {
        m_isDraggingWindow = false;
        return;
    }

    forEachVisibleWindowFrontToBack([&](Window& window) -> IteratorResult {
        if (event.type() == Event::Type::MouseDown) {
            if (windowTitleBarRect(window).contains(event.position())) {
                if (windowTitleBarCloseButtonRect(window).contains(event.position())) {
                    remove(window);
                    return IteratorResult::Break;
                }

                makeActive(window);
                onWindowTaskBarMouseDown(window, event.x(), event.y());
                return IteratorResult::Break;
            }
        }

        if (window.contains(event.position())) {
            if (event.type() == Event::Type::MouseDown) {
                makeActive(window);
            }
            MouseEvent mouseEventRelativeToWindow(event.type(), event.x() - window.rect().x(), event.y() - window.rect().y());
            window.event(mouseEventRelativeToWindow);
            return IteratorResult::Break;
        }

        return IteratorResult::Continue;
    });
}

void WindowManager::processPaintEvent(Event& event)
{
    // FIXME: handle outside of windows manager
    GUI::Screen::instance().fill(GUI::Colors::White);

    forEachVisibleWindowBackToFront([&](GUI::Window& window) -> GUI::IteratorResult {
        paintWindow(window, event);
        return GUI::IteratorResult::Continue;
    });

    paintTaskbar();

    // FIXME: handle outside of windows manager;
    GUI::Screen::instance().update();
}

void WindowManager::paintWindow(Window& window, Event& event)
{
    const bool isActiveWindow = m_activeWindow == &window;

    Painter painter;
    painter.drawFilledRect(windowFrameRect(window), Colors::Grey);
    painter.drawFilledRect(windowTitleBarRect(window), isActiveWindow ? ActiveWindowTitleBarColor : InactiveTitleBarColor);

    IntRect closeButtonRect = windowTitleBarCloseButtonRect(window);
    IntPoint closeButtonBitmapPos = closeButtonRect.position();
    closeButtonBitmapPos.moveBy(3, 3);
    painter.drawFilledRect(closeButtonRect, Colors::Grey);
    painter.drawCharacterBitmap(closeButtonBitmapPos, closeButtonBitmap, Colors::Black);

    painter.drawRect(windowTitleBarCloseButtonRect(window), Colors::Black);
    painter.drawText(windowTitleBarRect(window), window.title(), TextAlignment::Center, isActiveWindow ? ActiveWindowTitleBarTextColor : InactiveTitleBarTextColor);
    painter.drawRect(windowFrameRect(window), isActiveWindow ? Colors::Black : InactiveTitleBarColor);

    window.event(event);
}

void WindowManager::onWindowTaskBarMouseDown(Window& window, int x, int y)
{
    const auto& rect = window.rect();
    if (!windowTitleBarCloseButtonRect(window).contains(x, y)) {
        m_lastMouseDragPos = { x, y };
        m_isDraggingWindow = true;
    }
}

} // GUI