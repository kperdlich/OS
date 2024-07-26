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

static constexpr const int FrameBorder = 3;
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
    const auto& rect = windowOuterFrameRect(window);
    return { rect.x(), rect.y(), rect.width(), FrameBorder };
}

static Rect windowLeftOuterFrameRect(const Window& window)
{
    const auto& rect = windowOuterFrameRect(window);
    return { rect.x(), rect.y(), FrameBorder, rect.height() };
}

static Rect windowRightOuterFrameRect(const Window& window)
{
    const auto& rect = windowOuterFrameRect(window);
    return { rect.right() - FrameBorder, rect.y(), FrameBorder, rect.height() };
}

static Rect windowBottomOuterFrameRect(const Window& window)
{
    const auto& rect = windowOuterFrameRect(window);
    return { rect.x(), rect.bottom() - FrameBorder, rect.width(), FrameBorder };
}

static Rect windowResizeUpLeftRect(const Window& window)
{
    const Rect top = windowTopOuterFrameRect(window);
    const Rect left = windowLeftOuterFrameRect(window);
    return top.intersectRect(left);
}

static Rect windowResizeUpRightRect(const Window& window)
{
    const Rect top = windowTopOuterFrameRect(window);
    const Rect right = windowRightOuterFrameRect(window);
    return top.intersectRect(right);
}

static Rect windowResizeDownLeftRect(const Window& window)
{
    const Rect bottom = windowBottomOuterFrameRect(window);
    const Rect left = windowLeftOuterFrameRect(window);
    return bottom.intersectRect(left);
}

static Rect windowResizeDownRightRect(const Window& window)
{
    const Rect bottom = windowBottomOuterFrameRect(window);
    const Rect right = windowRightOuterFrameRect(window);
    return bottom.intersectRect(right);
}

static void paintTaskbar()
{
    // FIXME:
    // Painter painter;
    // painter.drawFilledRect(TaskbarRect, TaskbarColor);
}

void WindowManager::event(Event& event)
{
    if (event.isMouseEvent()) {
        return processMouseEvent(static_cast<MouseEvent&>(event));
    }

    if (event.isKeyboardEvent()) {
        if (m_activeWindow) {
            return m_activeWindow->event(event);
        }
        return CObject::event(event);
    }

    // Compose timer
    if (event.type() == Event::Type::Timer) {
        return compose();
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
        invalidate(*m_activeWindow);
        return;
    }

    if (m_isResizingWindow) {
        if (event.type() == Event::Type::MouseMove) {
            updateResizing(event.position());
        }

        if (event.type() == Event::Type::MouseUp) {
            m_isResizingWindow = false;
            m_resizeOrigin = {};
            m_resizeOption = ResizeDirection::None;
            m_resizeWindowStartRect = {};
            invalidate(*m_activeWindow);
        }
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

            if (insideResizeArea(window, event.position())) {
                makeActive(&window);
                startResizing(event.position());
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
    if (!windowTitleBarCloseButtonRect(window).contains(x, y)) {
        m_dragOrigin = { x, y };
        m_dragWindowOrigin = window.position();
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

void WindowManager::invalidateWindowLocalRect(Window& window, const Rect& rect)
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
    Rect bigDirtyRect;
    for (auto& dirtyRect : dirtyRects) {
        bigDirtyRect = bigDirtyRect.united(dirtyRect);
    }
    Painter painter(*m_backBuffer);
    painter.drawFilledRect(bigDirtyRect, Colors::White);

    forEachVisibleWindowBackToFront([&](Window& window) -> IteratorResult {
        if (!bigDirtyRect.intersects(windowOuterFrameRect(window))) {
            return IteratorResult::Continue;
        }
        Bitmap* const windowBitmap = window.backBuffer();
        if (!windowBitmap) {
            std::cout << "[compose] no bitmap for window: " << window.title() << std::endl;
            return IteratorResult::Continue;
        }

        paintWindowFrame(window);
        Painter painter(*m_backBuffer);
        // FIXME: Only blit pixels for updated rect
        painter.blit(window.position(), *windowBitmap);
        return IteratorResult::Continue;
    });

    // FIXME: Only copy updated pixels for updated rect
    ASSERT(m_backBuffer->size() == m_frontBuffer->size());
    ASSERT(m_backBuffer->format() == m_frontBuffer->format());
    char* src = m_backBuffer->data();
    char* dst = m_frontBuffer->data();
    ADS::memcpy(dst, src, m_frontBuffer->width() * m_frontBuffer->height() * m_frontBuffer->byteDensity());

    killTimer(m_composeTimer);
    m_composeTimer = -1;

    Screen::instance().present();
}

bool WindowManager::insideResizeArea(const Window& window, const Point& position) const
{
    return windowTopOuterFrameRect(window).contains(position)
        || windowBottomOuterFrameRect(window).contains(position)
        || windowLeftOuterFrameRect(window).contains(position)
        || windowRightOuterFrameRect(window).contains(position);
}

void WindowManager::startResizing(const Point& position)
{
    ASSERT(m_activeWindow);
    m_resizeOption = ResizeDirection::None;

    if (windowResizeUpLeftRect(*m_activeWindow).contains(position)) {
        m_resizeOption = ResizeDirection::UpLeft;
    } else if (windowResizeUpRightRect(*m_activeWindow).contains(position)) {
        m_resizeOption = ResizeDirection::UpRight;
    } else if (windowResizeDownLeftRect(*m_activeWindow).contains(position)) {
        m_resizeOption = ResizeDirection::DownLeft;
    } else if (windowResizeDownRightRect(*m_activeWindow).contains(position)) {
        m_resizeOption = ResizeDirection::DownRight;
    } else if (windowTopOuterFrameRect(*m_activeWindow).contains(position)) {
        m_resizeOption = ResizeDirection::Up;
    } else if (windowBottomOuterFrameRect(*m_activeWindow).contains(position)) {
        m_resizeOption = ResizeDirection::Down;
    } else if (windowLeftOuterFrameRect(*m_activeWindow).contains(position)) {
        m_resizeOption = ResizeDirection::Left;
    } else if (windowRightOuterFrameRect(*m_activeWindow).contains(position)) {
        m_resizeOption = ResizeDirection::Right;
    }

    if (m_resizeOption != ResizeDirection::None) {
        m_resizeOrigin = position;
        m_resizeWindowStartRect = m_activeWindow->rect();
        m_isResizingWindow = true;
    }

    invalidate(*m_activeWindow);
}

void WindowManager::updateResizing(const Point& position)
{
    ASSERT(m_activeWindow);

    const Point diff = position - m_resizeOrigin;
    const Point newPos = m_resizeWindowStartRect.position();
    const Size newSize = m_resizeWindowStartRect.size();

    int changeX = 0;
    int changeY = 0;
    int changeWidth = 0;
    int changeHeight = 0;

    switch (m_resizeOption) {
    case ResizeDirection::Up:
        changeY = diff.y();
        changeHeight = -diff.y();
        break;
    case ResizeDirection::Down:
        changeHeight = diff.y();
        break;
    case ResizeDirection::Left:
        changeX = diff.x();
        changeWidth = -diff.x();
        break;
    case ResizeDirection::Right:
        changeWidth = diff.x();
        break;
    case ResizeDirection::UpLeft:
        changeX = diff.x();
        changeWidth = -diff.x();
        changeY = diff.y();
        changeHeight = -diff.y();
        break;
    case ResizeDirection::UpRight:
        changeY = diff.y();
        changeHeight = -diff.y();
        changeWidth = diff.x();
        break;
    case ResizeDirection::DownLeft:
        changeHeight = diff.y();
        changeX = diff.x();
        changeWidth = -diff.x();
        break;
    case ResizeDirection::DownRight:
        changeHeight = diff.y();
        changeWidth = diff.x();
        break;
    default:
        ASSERT(false);
        break;
    }

    // FIXME: minSizeHint or minSize?
    const Size minSize = m_activeWindow->centralWidget()->minSizeHint();
    Rect newRect { newPos.x() + changeX, newPos.y() + changeY, { newSize.width() + changeWidth, newSize.height() + changeHeight } };

    // Don't resize window when minsize is hit
    if (newRect.height() <= minSize.height()) {
        if (changeY != 0)
            newRect.setY(m_activeWindow->rect().bottom() - minSize.height());
        newRect.setHeight(minSize.height());
    }
    if (newRect.width() <= minSize.width()) {
        if (changeX != 0)
            newRect.setX(m_activeWindow->rect().right() - minSize.width());
        newRect.setWidth(minSize.width());
    }

    m_activeWindow->resize(newRect.size());
    m_activeWindow->setPosition(newRect.position());
}

} // GUI