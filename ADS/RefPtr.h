//
// Created by n3dry on 03.08.24.
//

#pragma once

#include "Move.h"
#include "RefCounted.h"

namespace ADS {

// Non-atomic reference counted ptr (similar to std::shared_ptr).
template<typename T>
class RefPtr {
public:
    RefPtr() = default;

    RefPtr(ADS::nullptr_t) {};

    explicit RefPtr(T* ptr)
        : m_ptr(ptr)
        , m_refCounter(new RefCounted())
    {
    }

    RefPtr(const RefPtr& other)
        : m_ptr(other.m_ptr)
        , m_refCounter(other.m_refCounter)
    {
        if (m_refCounter)
            m_refCounter->addRef();
    }

    RefPtr(RefPtr&& other)
        : m_ptr(other.m_ptr)
        , m_refCounter(other.m_refCounter)
    {
        other.m_ptr = nullptr;
        other.m_refCounter = nullptr;
    }

    ~RefPtr()
    {
        clear();
    }

    RefPtr& operator=(const RefPtr& other)
    {
        if (this != &other) {
            clear();
            m_ptr = other.m_ptr;
            m_refCounter = other.m_refCounter;
            if (m_refCounter)
                m_refCounter->addRef();
        }
        return *this;
    }

    RefPtr& operator=(RefPtr&& other)
    {
        if (this != &other) {
            clear();
            m_ptr = other.m_ptr;
            m_refCounter = other.m_refCounter;
            other.m_ptr = nullptr;
            other.m_refCounter = nullptr;
        }
        return *this;
    }

    T& operator*()
    {
        ASSERT(m_ptr != nullptr);
        return *m_ptr;
    }

    const T& operator*() const
    {
        ASSERT(m_ptr != nullptr);
        return *m_ptr;
    }

    T* operator->()
    {
        return m_ptr;
    }

    const T* operator->() const
    {
        return m_ptr;
    }

    int refCount() const
    {
        return m_refCounter ? m_refCounter->refCount() : 0;
    }

    void clear()
    {
        if (!m_refCounter)
            return;

        m_refCounter->releaseRef();
        if (m_refCounter->refCount() <= 0) {
            // FIXME: add custom deleter.
            delete m_ptr;
            m_ptr = nullptr;
            delete m_refCounter;
            m_refCounter = nullptr;
        }
    }

    bool operator!() { return !m_ptr; }
    operator bool() { return m_ptr; }

    bool operator==(T* ptr) { return m_ptr == ptr; }
    bool operator!=(T* ptr) { return m_ptr != ptr; }

private:
    T* m_ptr { nullptr };
    RefCounted* m_refCounter { nullptr };
};

template<typename T, typename... Args>
RefPtr<T> makeRef(Args&&... args)
{
    return RefPtr<T>(new T(ADS::forward<Args>(args)...));
}

}
