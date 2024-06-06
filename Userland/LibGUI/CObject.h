//
// Created by n3dry on 13.05.24.
//

#pragma once

#include "Types.h"

namespace GUI {

class Event;

class CObject {
public:
    explicit CObject(CObject* parent = nullptr);
    virtual ~CObject();

    virtual bool event(Event& event);

    void setParent(CObject* parent);
    inline CObject* parent() { return m_parent; };

protected:
    void addChild(CObject& child);
    void removeChild(CObject& child);

protected:
    ADS::Vector<CObject*> m_children;
    CObject* m_parent { nullptr };
};

} // GUI
