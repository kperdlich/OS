//
// Created by n3dry on 21.09.22.
//

#pragma once

#include "Types.h"
#include "Window.h"

namespace GUI {

enum class IteratorResult {
    Continue,
    Break,
};

class WindowStack {
public:
    WindowStack() = default;
    ~WindowStack() = default;

    static WindowStack& the() {
        static WindowStack instance;
        return instance;
    }

    void add(Window* window);
    void makeActive(Window* window);
    void remove(Window* window);

    template<typename Callback>
    void forEachVisibleWindowBackToFont(Callback callback);

    template<typename Callback>
    void forEachVisibleWindowFontToBack(Callback callback);

    void onMouseMove(int x, int y);
    void onMouseDown(int key, int x, int y);
    void onMouseUp(int key, int x, int y);

private:
    ADS::Vector<Window*> m_windows;
    Window* m_activeWindow { nullptr };
};

template<typename Callback>
inline void WindowStack::forEachVisibleWindowBackToFont(Callback callback)
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
inline void WindowStack::forEachVisibleWindowFontToBack(Callback callback)
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
