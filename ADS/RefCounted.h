//
// Created by n3dry on 03.08.24.
//

#pragma once

namespace ADS {

class RefCounted {
public:
    void addRef()
    {
        ++m_refCount;
    }

    void releaseRef()
    {
        --m_refCount;
    }

    [[nodiscard]] int refCount() const
    {
        return m_refCount;
    }

    void addWeakRef()
    {
        ++m_weakRefCount;
    }

    void releaseWeakRef()
    {
        --m_weakRefCount;
    }

    [[nodiscard]] int weakRefCount() const
    {
        return m_weakRefCount;
    }

private:
    int m_refCount { 1 };
    int m_weakRefCount { 0 };
};

}
