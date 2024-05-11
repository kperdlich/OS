//
// Created by n3dry on 17.09.22.
//

#include "Window.h"
#include "WindowManager.h"

namespace GUI {

static const int TitleBarHeight = 20;
static const GUI::Color TitleBarColor { 190, 190, 190, 0xff };
static const GUI::Color ActiveWindowTitleBarColor { 90, 90, 90, 0xff };
static const GUI::Color ContentBackgroundColor { 0xff, 0xff, 0xff, 0xff };
static const int ContentMargin = 1;

Window::Window(IntRect rect)
    : m_rect(rect)
    , m_contentRect({ rect.x() + ContentMargin, rect.y() + TitleBarHeight + ContentMargin, rect.width() - (ContentMargin * 2), rect.height() - TitleBarHeight - (2 * ContentMargin)})
{
    m_closeButton = ADS::UniquePtr<Button>(new Button(nullptr, [&]() {
        GUI::WindowManager::the().remove(*this);
    }));

    m_hideButton = ADS::UniquePtr<Button>(new Button(nullptr, [&]() {
        hide();
    }));
}

void Window::render(Painter& painter, bool isActiveWindow) const
{
    // FIXME: rendering should be done in the windows manager.
    if (!m_isVisible)
        return;

    painter.drawRectangle(m_rect, GUI::Color(0x00, 0, 0, 0xff));
    painter.drawRectangle(titleBarRect(), isActiveWindow ? ActiveWindowTitleBarColor : TitleBarColor);
    m_closeButton->render(painter, { m_rect.x() + m_rect.width() - TitleBarHeight, m_rect.y(), TitleBarHeight, TitleBarHeight }, Color { 0xff, 0, 0, 0xff });
    m_closeButton->render(painter, { m_rect.x() + m_rect.width() - (TitleBarHeight * 2), m_rect.y(), TitleBarHeight, TitleBarHeight }, Color { 0, 0xff, 0, 0xff });

    painter.drawRectangle(m_contentRect, ContentBackgroundColor);
}

void Window::onMouseMove(int x, int y)
{
    if (m_isDragging) {
        const int deltaX = x - m_lastMouseMovePos.width();
        const int deltaY = y - m_lastMouseMovePos.height();
        m_rect.moveBy(deltaX, deltaY);
        m_contentRect.moveBy(deltaX, deltaY);
        m_lastMouseMovePos = { x, y };
        return;
    }

    if (IntRect { m_rect.x() + m_rect.width() - TitleBarHeight, m_rect.y(), TitleBarHeight, TitleBarHeight }.contains(x, y)) {
        m_closeButton->onMouseMove(x, y);
        return;
    }

    if (IntRect { m_rect.x() + m_rect.width() - (TitleBarHeight * 2), m_rect.y(), TitleBarHeight, TitleBarHeight }.contains(x, y)) {
        m_hideButton->onMouseMove(x, y);
        return;
    }
}

void Window::onMouseUp(int key, int x, int y)
{
    if (m_rect.contains(x, y) && m_isDragging) {
        m_isDragging = false;
        return;
    }

    if (IntRect { m_rect.x() + m_rect.width() - TitleBarHeight, m_rect.y(), TitleBarHeight, TitleBarHeight }.contains(x, y)) {
        m_closeButton->onMouseUp(key, x, y);
        return;
    }

    if (IntRect { m_rect.x() + m_rect.width() - (TitleBarHeight * 2), m_rect.y(), TitleBarHeight, TitleBarHeight }.contains(x, y)) {
        m_hideButton->onMouseUp(key, x, y);
        return;
    }
}

void Window::onMouseDown(int key, int x, int y)
{
    if (IntRect { m_rect.x() + m_rect.width() - TitleBarHeight, m_rect.y(), TitleBarHeight, TitleBarHeight }.contains(x, y)) {
        m_closeButton->onMouseDown(key, x, y);
        return;
    }

    if (IntRect { m_rect.x() + m_rect.width() - (TitleBarHeight * 2), m_rect.y(), TitleBarHeight, TitleBarHeight }.contains(x, y)) {
        m_hideButton->onMouseDown(key, x, y);
        return;
    }

    if (titleBarRect().contains(x, y)) {
        m_lastMouseMovePos = { x, y };
        m_isDragging = true;
        return;
    }
}
bool Window::hits(int x, int y)
{
    return m_rect.contains(x, y);
}

IntRect Window::titleBarRect() const
{
    return { m_rect.x(), m_rect.y(), m_rect.width(), TitleBarHeight };
}

} // GUI