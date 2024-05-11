//
// Created by n3dry on 21.09.22.
//

#include "WindowManager.h"
#include "Painter.h"

namespace GUI {

// FIXME get this from framebuffer
constexpr int width = 1024;
constexpr int height = 720;
constexpr int TaskbarHeight = 20;
// FIXME render everything window frame related in the window manager and not in the windows class
static const GUI::Color TaskbarColor { 190, 190, 190, 0xff };
static const IntRect TaskbarRect {0, height - TaskbarHeight, width, TaskbarHeight};

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
        if (window.hits(x, y)) {
            makeActive(window);
            window.onMouseDown(key, x, y);
            return IteratorResult::Break;
        }
        return IteratorResult::Continue;
    });
}

void WindowManager::onMouseMove(int x, int y)
{
    if (m_activeWindow) {
        m_activeWindow->onMouseMove(x, y);
    }
}

void WindowManager::onMouseUp(int key, int x, int y)
{
    forEachVisibleWindowFrontToBack([&](Window& window) -> IteratorResult {
        if (window.hits(x, y)) {
            window.onMouseUp(key, x, y);
            return IteratorResult::Break;
        }
        return IteratorResult::Continue;
    });
}
void WindowManager::render(Painter& painter)
{
    forEachVisibleWindowBackToFront([&](GUI::Window& window) -> GUI::IteratorResult {
        window.render(painter, m_activeWindow == &window);
        return GUI::IteratorResult::Continue;
    });

    paintTaskbar(painter);
}
void WindowManager::paintTaskbar(Painter& painter) const
{
    painter.drawRectangle(TaskbarRect, TaskbarColor);
}

} // GUI