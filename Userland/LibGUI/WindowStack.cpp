//
// Created by n3dry on 21.09.22.
//

#include "WindowStack.h"
#include <algorithm>

namespace GUI {

void WindowStack::add(Window& window)
{
    m_windows.emplace_back(&window);
}

void WindowStack::remove(Window& window)
{
    // This sucks!!!!
    for (auto iter = m_windows.begin(); iter != m_windows.end();) {
        if (*iter == &window) {
            m_windows.erase(iter++);
            break;
        } else {
            ++iter;
        }
    }
}

void WindowStack::makeActive(Window& window)
{
    remove(window);
    add(window);
    m_activeWindow = &window;
}

void WindowStack::onMouseDown(int key, int x, int y)
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

void WindowStack::onMouseMove(int x, int y)
{
    if (m_activeWindow) {
        m_activeWindow->onMouseMove(x, y);
    }
}

void WindowStack::onMouseUp(int key, int x, int y)
{
    forEachVisibleWindowFrontToBack([&](Window& window) -> IteratorResult {
        if (window.hits(x, y)) {
            window.onMouseUp(key, x, y);
            return IteratorResult::Break;
        }
        return IteratorResult::Continue;
    });
}


} // GUI