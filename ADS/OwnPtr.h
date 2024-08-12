//
// Created by n3dry on 30.07.24.
//

#pragma once

#include "Assert.h"

namespace ADS {

// Similar to std::unique_ptr
template<typename T>
class OwnPtr {
public:
    OwnPtr() = default;

    OwnPtr(ADS::nullptr_t) {};

    explicit OwnPtr(T* ptr)
        : m_ptr(ptr)
    {
    }

    OwnPtr(OwnPtr<T>&& other)
        : m_ptr(other.release())
    {
    }

    template<typename Other>
    OwnPtr(OwnPtr<Other>&& other)
        : m_ptr(static_cast<T*>(other.release()))
    {
    }

    ~OwnPtr()
    {
        clear();
    }

    const T* ptr() const { return m_ptr; }
    T* ptr() { return m_ptr; }

    T* release()
    {
        T* const ptr = m_ptr;
        m_ptr = nullptr;
        return ptr;
    }

    OwnPtr& operator=(OwnPtr&& other)
    {
        if (this != &other) {
            delete m_ptr;
            m_ptr = other.release();
        }
        return *this;
    }

    OwnPtr& operator=(T* other)
    {
        if (m_ptr != other) {
            delete m_ptr;
            m_ptr = other;
        }
        return *this;
    }

    void clear()
    {
        // FIXME: Add custom deleter
        delete m_ptr;
        m_ptr = nullptr;
    }

    T* operator->() { return m_ptr; }
    const T* operator->() const { return m_ptr; }

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

    bool operator!() const { return !m_ptr; }
    operator bool() const { return m_ptr; }

    bool operator==(T* ptr) const { return m_ptr == ptr; }
    bool operator!=(T* ptr) const { return m_ptr != ptr; }

private:
    T* m_ptr { nullptr };
};

template<typename T, typename... Args>
OwnPtr<T> makeOwn(Args&&... args)
{
    return OwnPtr<T>(new T(ADS::forward<Args>(args)...));
}

}
