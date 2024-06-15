//
// Created by n3dry on 13.05.24.
//

#include "CObject.h"
#include "Application.h"
#include "Event.h"
#include "TimerManager.h"

namespace GUI {

CObject::CObject(CObject* parent)
    : m_parent(parent)
{
    if (parent) {
        parent->addChild(*this);
    }
}

CObject::~CObject()
{
    if (m_parent) {
        m_parent->removeChild(*this);
    }

    const ADS::Vector<CObject*> childrenToRemove = std::move(m_children);
    for (CObject* child : childrenToRemove) {
        delete child;
    }
}

bool CObject::event(Event& event)
{
    switch (event.type()) {
    case Event::Type::Timer:
        onTimerEvent(static_cast<TimerEvent&>(event));
        return true;
    case Event::Type::DeferredDestroy:
        delete this;
        return true;
    }

    return false;
}

int CObject::startTimer(int intervalMs)
{
    return TimerManager::instance().startTimer(intervalMs, *this);
}

void CObject::killTimer(int id)
{
    TimerManager::instance().killTimer(id);
}

void CObject::addChild(CObject& child)
{
    m_children.emplace_back(&child);
}

void CObject::removeChild(CObject& child)
{
    m_children.erase(
        std::remove_if(m_children.begin(), m_children.end(),
            [&child](CObject* obj) { return obj == &child; }),
        m_children.end());
}

void CObject::setParent(CObject* parent)
{
    if (parent == m_parent) {
        return;
    }

    if (m_parent) {
        m_parent->removeChild(*this);
    }

    m_parent = parent;

    if (m_parent) {
        m_parent->addChild(*this);
    }
}

void CObject::onTimerEvent(TimerEvent& event)
{
}

bool CObject::isWidgetType() const
{
    return false;
}

bool CObject::isWindowType() const
{
    return false;
}

void CObject::deleteLater()
{
    Application::instance().postEvent(this, ADS::UniquePtr<Event>(new Event(Event::Type::DeferredDestroy)));
}

} // GUI