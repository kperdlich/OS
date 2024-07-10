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

class WindowManager final : public CObject {
public:
    static WindowManager& instance()
    {
        static WindowManager instance;
        return instance;
    }

    void show(Window& window);
    void makeActive(Window* window);
    void remove(Window& window);

    virtual bool event(Event& event) override;

    template<typename Callback>
    void forEachVisibleWindowBackToFront(Callback callback);

    template<typename Callback>
    IteratorResult forEachVisibleWindowFrontToBack(Callback callback);

    Window* activeWindow() const { return m_activeWindow; }

    void setMouseGrabbedWidget(Widget& widget);
    void releaseMouseGrabbedWidget();


    void invalidateWindowRect(Window& window, const Rect& rect);
    void invalidate(Window& window);
    void invalidate(const Rect& rect);
    void hide(Window& window);

    void compose();
    void flushPainting();

private:
    void processMouseEvent(MouseEvent& event);

    WindowManager() = default;

    void onWindowTaskBarMouseDown(Window& window, int x, int y);
    void paintWindowFrame(Window& window);
    void closeWindow(Window& window);
    bool insideWindowResizeArea(const Window& window, const Point& position) const;

private:
    ADS::UniquePtr<Bitmap> m_frontBuffer;
    ADS::UniquePtr<Bitmap> m_backBuffer;
    ADS::Vector<Window*> m_windows;
    ADS::Vector<Rect> m_dirtyRects;
    Window* m_activeWindow { nullptr };
    Widget* m_mouseGrabbedWidget { nullptr };
    Point m_dragOrigin;
    Point m_dragWindowOrigin;
    int m_composeTimer { -1 };
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
inline IteratorResult WindowManager::forEachVisibleWindowFrontToBack(Callback callback)
{
    for (auto it = m_windows.rbegin(); it != m_windows.rend(); ++it) {
        if (!(*it)->isVisible())
            continue;
        const auto result = callback(**it);
        if (result == IteratorResult::Break)
            return IteratorResult::Break;
    }
    return IteratorResult::Continue;
}

} // GUI
