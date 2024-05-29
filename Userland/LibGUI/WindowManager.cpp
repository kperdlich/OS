//
// Created by n3dry on 21.09.22.
//

#include "WindowManager.h"
#include "CharacterBitmap.h"
#include "Painter.h"
#include "Widget.h"

namespace GUI {

static constexpr const int TitleBarHeight = 20;
static constexpr const int TitleBarButtonsMargin = 2;
static constexpr const int TitleBarButtonSize = TitleBarHeight - (2 * TitleBarButtonsMargin);
static const GUI::Color InactiveTitleBarColor { 140, 140, 140, 0xff };
static const GUI::Color InactiveTitleBarTextColor { 234, 233, 233, 0xff };
static const GUI::Color ActiveWindowTitleBarColor { 0, 0, 104, 0xff };
static const GUI::Color ActiveWindowTitleBarTextColor { 0xff, 0xff, 0xff, 0xff };

// FIXME get this from framebuffer
static constexpr const int width = 1024;
static constexpr const int height = 720;

static const IntSize closeButtonCharSize {11, 9};
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

void WindowManager::onMouseDown(int key, int x, int y)
{
    forEachVisibleWindowFrontToBack([&](Window& window) -> IteratorResult {
        if (windowTitleBarRect(window).contains(x, y)) {
            makeActive(window);
            onWindowTaskBarMouseDown(window, x, y);
            return IteratorResult::Break;
        }

        if (window.contains(x, y)) {
            makeActive(window);
            window.onMouseDown(key, x, y);
            return IteratorResult::Break;
        }

        return IteratorResult::Continue;
    });
}

void WindowManager::onMouseMove(int x, int y)
{
    if (m_isDraggingWindow) {
        const int deltaX = x - m_lastMouseDragPos.width();
        const int deltaY = y - m_lastMouseDragPos.height();
        m_activeWindow->moveBy(deltaX, deltaY);
        m_lastMouseDragPos = { x, y };
        return;
    }

    if (m_activeWindow) {
        m_activeWindow->onMouseMove(x, y);
    }
}

void WindowManager::onMouseUp(int key, int x, int y)
{
    if (m_isDraggingWindow) {
        m_isDraggingWindow = false;
        return;
    }

    forEachVisibleWindowFrontToBack([&](Window& window) -> IteratorResult {
        const auto& rect = window.rect();
        if (windowTitleBarCloseButtonRect(window).contains(x, y)) {
            remove(window);
            return IteratorResult::Break;
        }

        if (window.contains(x, y)) {
            window.onMouseUp(key, x, y);
            return IteratorResult::Break;
        }

        return IteratorResult::Continue;
    });
}
void WindowManager::paint()
{
    forEachVisibleWindowBackToFront([&](GUI::Window& window) -> GUI::IteratorResult {
        paintWindow(window);
        return GUI::IteratorResult::Continue;
    });

    paintTaskbar();
}

void WindowManager::paintWindow(Window& window)
{
    const bool isActiveWindow = m_activeWindow == &window;

    Painter painter;
    painter.drawFilledRect(windowFrameRect(window), Color { 172, 172, 172, 0xff });
    painter.drawFilledRect(windowTitleBarRect(window), isActiveWindow ? ActiveWindowTitleBarColor : InactiveTitleBarColor);

    IntRect closeButtonRect = windowTitleBarCloseButtonRect(window);
    IntPoint closeButtonBitmapPos = closeButtonRect.position();
    closeButtonBitmapPos.moveBy(3, 3);
    painter.drawFilledRect(closeButtonRect, InactiveTitleBarColor);
    painter.drawCharacterBitmap(closeButtonBitmapPos, closeButtonBitmap, Color { 0, 0, 0, 0xff });

    painter.drawRect(windowTitleBarCloseButtonRect(window), Color { 0x00, 0, 0, 0xff });
    painter.drawText(windowTitleBarRect(window), window.title(), TextAlignment::Center, isActiveWindow ? ActiveWindowTitleBarTextColor : InactiveTitleBarTextColor);
    painter.drawRect(windowFrameRect(window), isActiveWindow ? Color { 0, 0, 0, 0xff } : InactiveTitleBarColor);

    window.onPaint();
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