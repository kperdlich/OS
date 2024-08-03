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

    int refCount() const
    {
        return m_refCount;
    }

private:
    int m_refCount { 1 };
};

}
