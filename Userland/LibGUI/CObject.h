//
// Created by n3dry on 13.05.24.
//

#pragma once

#include "Types.h"
#include "Event.h"

namespace GUI {

class CObject {
public:
    explicit CObject(CObject* parent = nullptr);
    virtual ~CObject();

    virtual void event(Event& event);
    virtual void onTimerEvent(TimerEvent& event);
    virtual bool isWidgetType() const;
    virtual bool isWindowType() const;

    int startTimer(int intervalMs);
    void killTimer(int id);

    void setParent(CObject* parent);
    CObject* parent() const { return m_parent; };

protected:
    void addChild(CObject& child);
    void removeChild(CObject& child);

protected:
    ADS::Vector<CObject*> m_children;
    CObject* m_parent { nullptr };
};

} // GUI
