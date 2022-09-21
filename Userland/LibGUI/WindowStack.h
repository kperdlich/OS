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

    void add(Window* window);
    void makeActive(Window* window);
    void remove(Window* window);

    template<typename Callback>
    void forEachWindowBackToFont(Callback callback);

    template<typename Callback>
    void forEachWindowFontToBack(Callback callback);

    void onMouseMove(int x, int y);
    void onMouseDown(int key, int x, int y);
    void onMouseUp(int key, int x, int y);

private:
    ADS::Vector<Window*> m_windows;
    Window* m_activeWindow { nullptr };
};

template<typename Callback>
inline void WindowStack::forEachWindowBackToFont(Callback callback)
{
    for (auto& m_window : m_windows) {
        const auto result = callback(*m_window);
        if (result == IteratorResult::Break)
            break;
    }
}

template<typename Callback>
inline void WindowStack::forEachWindowFontToBack(Callback callback)
{
    for (auto it = m_windows.rbegin(); it != m_windows.rend(); ++it) {
        const auto result = callback(**it);
        if (result == IteratorResult::Break)
            break;
    }
}

} // GUI
