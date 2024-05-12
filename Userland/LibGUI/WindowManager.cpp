//
// Created by n3dry on 21.09.22.
//

#include "WindowManager.h"
#include "Painter.h"

namespace GUI {

static const int TitleBarHeight = 20;
static const GUI::Color TitleBarColor { 190, 190, 190, 0xff };
static const GUI::Color ActiveWindowTitleBarColor { 90, 90, 90, 0xff };

// FIXME get this from framebuffer
constexpr int width = 1024;
constexpr int height = 720;

constexpr int TaskbarHeight = 20;
static const GUI::Color TaskbarColor { 190, 190, 190, 0xff };
static const IntRect TaskbarRect { 0, height - TaskbarHeight, width, TaskbarHeight };

static IntRect windowTitleBarCloseButtonRect(const Window& window)
{
    const auto& rect = window.rect();
    return { rect.x() + rect.width() - TitleBarHeight, rect.y(), TitleBarHeight, TitleBarHeight };
}

static IntRect windowTitleBarRect(const Window& window)
{
    const auto& rect = window.rect();
    return { rect.x(), rect.y(), rect.width(), TitleBarHeight };
}

static void paintTaskbar(Painter& painter)
{
    painter.drawRectangle(TaskbarRect, TaskbarColor);
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
        if (window.contains(x, y)) {
            window.onMouseUp(key, x, y);
            return IteratorResult::Break;
        }

        return IteratorResult::Continue;
    });
}
void WindowManager::paint(Painter& painter)
{
    forEachVisibleWindowBackToFront([&](GUI::Window& window) -> GUI::IteratorResult {
        paintWindow(painter, window);
        return GUI::IteratorResult::Continue;
    });

    paintTaskbar(painter);
}

void WindowManager::paintWindow(Painter& painter, const Window& window)
{
    painter.drawRectangle(window.rect(), GUI::Color(0x00, 0, 0, 0xff));
    painter.drawRectangle(windowTitleBarRect(window), m_activeWindow == &window ? ActiveWindowTitleBarColor : TitleBarColor);

    painter.drawRectangle(windowTitleBarCloseButtonRect(window), Color { 0xff, 0, 0, 0xff });
}

void WindowManager::onWindowTaskBarMouseDown(Window& window, int x, int y)
{
    const auto& rect = window.rect();
    if (IntRect { rect.x() + rect.width() - TitleBarHeight, rect.y(), TitleBarHeight, TitleBarHeight }.contains(x, y)) {
        remove(window);
        return;
    }

    m_lastMouseDragPos = { x, y };
    m_isDraggingWindow = true;
}

} // GUI