//
// Created by n3dry on 21.09.22.
//

#pragma once

#include "Event.h"
#include "Types.h"
#include "Window.h"

namespace GUI {

enum class IteratorResult {
    Continue,
    Break,
};

class Painter;

class WindowManager final {
public:
    static WindowManager& the()
    {
        static WindowManager instance;
        return instance;
    }

    void add(Window& window);
    void makeActive(Window& window);
    void remove(Window& window);

    template<typename Callback>
    void forEachVisibleWindowBackToFront(Callback callback);

    template<typename Callback>
    void forEachVisibleWindowFrontToBack(Callback callback);

    void onMouseMove(int x, int y);
    void onMouseDown(int key, int x, int y);
    void onMouseUp(int key, int x, int y);
    void onKeyDown(const KeyEvent& event);
    void onKeyUp(const KeyEvent& event);
    void paint();

private:
    WindowManager() = default;

    void onWindowTaskBarMouseDown(Window& window, int x, int y);
    void paintWindow(Window& window);

private:
    ADS::Vector<Window*> m_windows;
    Window* m_activeWindow { nullptr };

    IntSize m_lastMouseDragPos { 0, 0 };
    bool m_isDraggingWindow { false };
};

template<typename Callback>
inline void WindowManager::forEachVisibleWindowBackToFront(Callback callback)
{
    for (auto& window : m_windows) {
        if (!window->isVisible())
            continue;
        const auto result = callback(*window);
        if (result == IteratorResult::Break)
            break;
    }
}

template<typename Callback>
inline void WindowManager::forEachVisibleWindowFrontToBack(Callback callback)
{
    for (auto it = m_windows.rbegin(); it != m_windows.rend(); ++it) {
        if (!(*it)->isVisible())
            continue;
        const auto result = callback(**it);
        if (result == IteratorResult::Break)
            break;
    }
}

} // GUI
