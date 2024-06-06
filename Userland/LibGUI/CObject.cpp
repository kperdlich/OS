//
// Created by n3dry on 13.05.24.
//

#include "CObject.h"

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

    const ADS::Vector<CObject*> children_to_remove = std::move(m_children);
    for (CObject* child : children_to_remove) {
        delete child;
    }
}

bool CObject::event(Event& event)
{
    return false;
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

} // GUI