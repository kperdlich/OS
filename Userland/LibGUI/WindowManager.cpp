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

static constexpr const int FrameBorder = 2;
static constexpr const int TaskbarHeight = 20;
static const GUI::Color TaskbarColor { 190, 190, 190, 0xff };
static const Rect TaskbarRect { 0, height - TaskbarHeight, width, TaskbarHeight };

static Rect windowOuterFrameRect(const Window& window)
{
    const auto& rect = window.rect();
    return { rect.x() - FrameBorder, rect.y() - TitleBarHeight - FrameBorder, rect.width() + (2 * FrameBorder), rect.height() + TitleBarHeight + (2 * FrameBorder) };
}

static Rect windowTitleBarCloseButtonRect(const Window& window)
{
    const auto& rect = window.rect();
    return { rect.x() + rect.width() - TitleBarButtonSize - TitleBarButtonsMargin, rect.y() - TitleBarHeight + TitleBarButtonsMargin, TitleBarButtonSize, TitleBarButtonSize };
}

static Rect windowTitleBarRect(const Window& window)
{
    const auto& rect = window.rect();
    return { rect.x(), rect.y() - TitleBarHeight, rect.width(), TitleBarHeight };
}

static Rect windowTopOuterFrameRect(const Window& window)
{
    const auto& rect = window.rect();
    return { rect.x() - FrameBorder, rect.y() - TitleBarHeight - FrameBorder, rect.width() + (2 * FrameBorder), FrameBorder };
}

static Rect windowLeftOuterFrameRect(const Window& window)
{
    const auto& rect = window.rect();
    return { rect.x() - FrameBorder, rect.y() - TitleBarHeight, FrameBorder, rect.height() + TitleBarHeight };
}

static Rect windowRightOuterFrameRect(const Window& window)
{
    const auto& rect = window.rect();
    return { rect.x() + rect.width(), rect.y() - TitleBarHeight, FrameBorder, rect.height() + TitleBarHeight };
}

static Rect windowBottomOuterFrameRect(const Window& window)
{
    const auto& rect = window.rect();
    return { rect.x() - FrameBorder, rect.y() + rect.height(), rect.width() + (2 * FrameBorder), FrameBorder };
}

static void paintTaskbar()
{
    // FIXME:
    // Painter painter;
    // painter.drawFilledRect(TaskbarRect, TaskbarColor);
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

    // Compose timer
    if (event.type() == Event::Type::Timer) {
        compose();
        return true;
    }

    return CObject::event(event);
}

void WindowManager::show(Window& window)
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

    Window* const previousWindow = m_activeWindow;
    m_activeWindow = window;

    if (previousWindow)
        invalidate(*previousWindow);

    if (m_activeWindow) {
        if (m_activeWindow->focusedWidget())
            Application::instance().postEvent(m_activeWindow->focusedWidget(), ADS::UniquePtr<FocusEvent>(new FocusEvent(Event::Type::FocusIn, FocusReason::ActiveWindow)));
        invalidate(*m_activeWindow);
    }
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
        invalidate(*m_activeWindow);
        Point pos = m_dragWindowOrigin;
        pos.moveBy(event.x() - m_dragOrigin.x(), event.y() - m_dragOrigin.y());
        m_activeWindow->setPosition(pos);
        invalidate(*m_activeWindow);
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

            if (insideWindowResizeArea(window, event.position())) {
                std::cout << "[WindowManager::processMouseEvent] on resize area " << event.position().toString() << std::endl;
                // FIXME: implement window resize
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
    ASSERT(m_backBuffer != nullptr);

    const bool isActiveWindow = m_activeWindow == &window;

    Painter painter(*m_backBuffer);
    painter.drawFilledRect(windowTitleBarRect(window), isActiveWindow ? ActiveWindowTitleBarColor : InactiveTitleBarColor);

    Rect closeButtonRect = windowTitleBarCloseButtonRect(window);
    Point closeButtonBitmapPos = closeButtonRect.position();
    closeButtonBitmapPos.moveBy(3, 3);
    painter.drawFilledRect(closeButtonRect, Colors::Grey);
    painter.drawCharacterBitmap(closeButtonBitmapPos, closeButtonBitmap, Colors::Black);

    painter.drawRect(windowTitleBarCloseButtonRect(window), Colors::Black);
    painter.drawText(windowTitleBarRect(window), window.title(), Alignment::Center, isActiveWindow ? ActiveWindowTitleBarTextColor : InactiveTitleBarTextColor);

    painter.drawFilledRect(windowTopOuterFrameRect(window), Colors::DarkGrey);
    painter.drawFilledRect(windowBottomOuterFrameRect(window), Colors::DarkGrey);
    painter.drawFilledRect(windowLeftOuterFrameRect(window), Colors::DarkGrey);
    painter.drawFilledRect(windowRightOuterFrameRect(window), Colors::DarkGrey);
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
    Rect absoluteRect = rect;
    absoluteRect.moveBy(window.position());
    invalidate(absoluteRect);

    Application::instance().postEvent(&window, ADS::UniquePtr<PaintEvent>(new PaintEvent(rect)));
}

void WindowManager::invalidate(Window& window)
{
    invalidate(windowOuterFrameRect(window));
}

void WindowManager::invalidate(const Rect& rect)
{
    for (auto& dirtyRect : m_dirtyRects) {
        if (dirtyRect == rect)
            return;
    }

    m_dirtyRects.push_back(rect);

    if (m_composeTimer < 0)
        m_composeTimer = startTimer(1000 / 60); // 60hz
}

void WindowManager::hide(Window& window)
{
    invalidate(window);
}

void WindowManager::compose()
{
    if (m_dirtyRects.empty())
        return;

    if (!m_frontBuffer)
        m_frontBuffer = Bitmap::createWrapper(Screen::instance().framebuffer());

    if (!m_backBuffer)
        m_backBuffer = Bitmap::createFrom(*m_frontBuffer);

    const auto start = std::chrono::steady_clock::now();
    flushPainting();
    const auto end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
    std::cout << "compose: time " << end.count() << " ms" << std::endl;
}

void WindowManager::flushPainting()
{
    ASSERT(m_backBuffer != nullptr);
    const ADS::Vector<Rect> dirtyRects = std::move(m_dirtyRects);

    // Draw Background by default
    for (auto& dirtyRect : dirtyRects) {
        Painter painter(*m_backBuffer);
        painter.drawFilledRect(dirtyRect, Colors::White);
    }

    forEachVisibleWindowBackToFront([&](Window& window) -> IteratorResult {
        for (auto& dirtyRect : dirtyRects) {
            if (!dirtyRect.intersects(windowOuterFrameRect(window))) {
                continue;
            }
            Bitmap* windowBackBuffer = window.backBuffer();
            if (!windowBackBuffer)
                return IteratorResult::Continue;

            Painter painter(*m_backBuffer);
            // FIXME: Only blit pixels for updated rect
            painter.blit(window.position(), *windowBackBuffer);
            paintWindowFrame(window);
            return IteratorResult::Continue;
        }
        return IteratorResult::Continue;
    });

    // FIXME: Only copy updated pixels for updated rect
    ASSERT(m_backBuffer->size() == m_frontBuffer->size());
    ASSERT(m_backBuffer->format() == m_frontBuffer->format());
    char* src = m_backBuffer->data();
    char* dst = m_frontBuffer->data();
    ADS::memcpy(dst, src, m_frontBuffer->width() * m_frontBuffer->height() * m_frontBuffer->byteDensity());

    Screen::instance().present();
}

bool WindowManager::insideWindowResizeArea(const Window& window, const Point& position) const
{
    return windowTopOuterFrameRect(window).contains(position)
        || windowBottomOuterFrameRect(window).contains(position)
        || windowLeftOuterFrameRect(window).contains(position)
        || windowRightOuterFrameRect(window).contains(position);
}

} // GUI