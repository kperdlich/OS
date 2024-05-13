//
// Created by n3dry on 13.05.24.
//

#pragma once

#include "Types.h"

namespace GUI {

class CObject {
public:
    explicit CObject(CObject* parent = nullptr);
    virtual ~CObject();

    void setParent(CObject* parent);
    inline CObject* parent() { return m_parent; };

private:
    void addChild(CObject& child);
    void removeChild(CObject& child);

private:
    ADS::Vector<CObject*> m_children;
    CObject* m_parent { nullptr };
};

} // GUI
