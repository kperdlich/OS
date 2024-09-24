//
// Created by n3dry on 14.08.24.
//

#pragma once

#include "Types.h"
#include "Vector.h"

#if 1
#include <ostream>
#endif

namespace ADS {

template<typename T>
class BasicString {
public:
    BasicString() = default;

    explicit BasicString(const T value)
    {
        m_capacity = 2;
        m_charBuffer = new T[m_capacity] { 0 };
        m_length = 1;
        m_charBuffer[0] = value;
    }

    BasicString(const T* value)
    {
        if (!value)
            return;

        m_length = strlen(value);
        if (m_length > 0) {
            m_capacity = m_length + 1;
            m_charBuffer = new T[m_capacity] { 0 };
            memcpy(m_charBuffer, value, m_length);
        }
    }

    BasicString(const T* value, size_t length)
    {
        if (!value)
            return;

        m_length = length;
        if (m_length > 0) {
            m_capacity = m_length + 1;
            m_charBuffer = new T[m_capacity] { 0 };
            memcpy(m_charBuffer, value, m_length);
        }
    }

    BasicString(const BasicString& other)
    {
        if (!other.isEmpty()) {
            m_capacity = other.m_capacity;
            m_length = other.m_length;
            m_charBuffer = new T[m_capacity] { 0 };
            memcpy(m_charBuffer, other.m_charBuffer, other.length());
        }
    }

    BasicString(BasicString&& other)
    {
        m_length = other.m_length;
        other.m_length = 0;
        m_capacity = other.m_capacity;
        other.m_capacity = 0;
        m_charBuffer = other.m_charBuffer;
        other.m_charBuffer = nullptr;
    }

    BasicString& operator=(const BasicString& other)
    {
        if (this == &other)
            return *this;

        clear();
        if (!other.isEmpty()) {
            m_length = other.m_length;
            // FIXME: Try to re-use the current buffer instead of allocating a new one?
            m_capacity = other.m_capacity;
            m_charBuffer = new T[m_capacity] { 0 };
            memcpy(m_charBuffer, other.m_charBuffer, other.length());
        }
        return *this;
    }

    BasicString& operator=(BasicString&& other)
    {
        if (this == &other)
            return *this;

        clear();
        if (!other.isEmpty()) {
            m_capacity = other.m_capacity;
            other.m_capacity = 0;
            m_charBuffer = other.m_charBuffer;
            other.m_charBuffer = nullptr;
            m_length = other.m_length;
            other.m_length = 0;
        }
        return *this;
    }

    BasicString& operator=(const T value)
    {
        if (m_capacity < 2) {
            m_capacity = 2;
            delete[] m_charBuffer;
            m_charBuffer = new T[m_capacity] { 0 };
        }
        m_charBuffer[0] = value;
        m_charBuffer[1] = 0;
        m_length = 1;
        return *this;
    }

    BasicString& operator=(const T* ptr)
    {
        if (!ptr)
            return *this;

        const size_t len = strlen(ptr);
        if (len == 0) {
            memset(m_charBuffer, 0, length());
            m_length = 0;
            return *this;
        }

        if (len + 1 > m_capacity) {
            m_capacity = len + 1;
            delete[] m_charBuffer;
            m_charBuffer = new T[m_capacity] { 0 };
        }

        m_length = len;
        memcpy(m_charBuffer, ptr, len);
        return *this;
    }

    const T& operator[](size_t index) const
    {
        ASSERT(index >= 0 && index < m_length);
        return m_charBuffer[index];
    }

    T& operator[](size_t index)
    {
        ASSERT(index >= 0 && index < m_length);
        return m_charBuffer[index];
    }

    size_t capacity() const
    {
        return m_capacity;
    }

    size_t length() const
    {
        return m_length;
    }

    bool isEmpty() const
    {
        return m_length == 0;
    }

    void clear()
    {
        delete[] m_charBuffer;
        m_charBuffer = nullptr;
        m_length = 0;
        m_capacity = 0;
    }

    bool operator==(const BasicString& other) const
    {
        if (length() != other.length())
            return false;

        return memcmp(m_charBuffer, other.m_charBuffer, length()) == 0;
    }

    bool operator!=(const BasicString& other) const
    {
        return !(*this == other);
    }

    void append(const BasicString& str)
    {
        if (length() + str.length() + 1 > capacity())
            reserve(length() + str.length());

        memcpy(&m_charBuffer[length()], str.m_charBuffer, str.length());
        m_length += str.length();
    }

    void append(const T* ptr)
    {
        if (!ptr)
            return;

        const size_t len = strlen(ptr);
        if (len == 0)
            return;

        if (length() + len + 1 > capacity())
            reserve(length() + len);

        memcpy(&m_charBuffer[length()], ptr, len);
        m_length += len;
    }

    void appendChar(const T& character)
    {
        if (length() + 2 > capacity())
            reserve(length() + 1);

        m_charBuffer[length()] = character;
        m_length += 1;
    }

    void appendFloat(float value)
    {
        // FIXME: This is not ideal
        const int length = snprintf(nullptr, 0, "%f", value);
        char* buffer = new char[length + 1] { 0 };
        snprintf(buffer, length + 1, "%f", value);
        append(buffer);
        delete[] buffer;
    }

    void appendInt(int32 value)
    {
        // FIXME: This is not ideal
        const int length = snprintf(nullptr, 0, "%d", value);
        char* buffer = new char[length + 1] { 0 };
        snprintf(buffer, length + 1, "%d", value);
        append(buffer);
        delete[] buffer;
    }

    BasicString substring(size_t start, size_t len) const
    {
        if (isEmpty())
            return {};

        const size_t validatedLength = min(start + len, length() - start);
        return { &m_charBuffer[start], validatedLength };
    }

    BasicString substring(size_t start) const
    {
        ASSERT(start < length());
        return { &m_charBuffer[start] };
    }

    void reserve(size_t newCapacity)
    {
        if (newCapacity == capacity())
            return;

        ASSERT(newCapacity >= length() + 1);
        T* newBuffer = new T[newCapacity + 1] { 0 };
        if (m_charBuffer) {
            memcpy(newBuffer, m_charBuffer, length());
            delete[] m_charBuffer;
        }
        m_charBuffer = newBuffer;
        m_capacity = newCapacity;
    }

    bool startsWith(const BasicString& str) const
    {
        if (str.isEmpty())
            return false;

        if (isEmpty())
            return false;

        if (length() < str.length())
            return false;

        for (size_t i = 0; i < str.length(); ++i) {
            if (str[i] != m_charBuffer[i])
                return false;
        }

        return true;
    }

    bool endsWith(const BasicString& str) const
    {
        if (str.isEmpty())
            return false;

        if (isEmpty())
            return false;

        if (length() < str.length())
            return false;

        for (size_t i = length() - str.length(); i < str.length(); ++i) {
            if (str[i] != m_charBuffer[i])
                return false;
        }

        return true;
    }

    BasicString toUpper() const
    {
        BasicString uppercaseStr;
        uppercaseStr.reserve(length());
        constexpr const char offset = 'a' - 'A';
        for (size_t i = 0; i < length(); ++i) {
            const char c = m_charBuffer[i];
            uppercaseStr.appendChar(m_charBuffer[i] - (c >= 'a' && c <= 'z' ? offset : 0));
        }
        return uppercaseStr;
    }

    BasicString toLower() const
    {
        BasicString lowercaseStr;
        lowercaseStr.reserve(length());
        constexpr const char offset = 'a' - 'A';
        for (size_t i = 0; i < length(); ++i) {
            const char c = m_charBuffer[i];
            lowercaseStr.appendChar(m_charBuffer[i] + (c >= 'A' && c <= 'Z' ? offset : 0));
        }
        return lowercaseStr;
    }

    TEST::Vector<BasicString> split(const char delimiter) const
    {
        TEST::Vector<BasicString> tokens;
        BasicString buffer;
        buffer.reserve(length());
        for (size_t i = 0; i < m_length; ++i) {
            const T c = m_charBuffer[i];
            if (c == delimiter) {
                if (!buffer.isEmpty()) {
                    tokens.pushBack(buffer);
                    buffer = "";
                }
            } else {
                buffer.appendChar(c);
            }
        }

        if (!buffer.isEmpty()) {
            tokens.pushBack(buffer);
        }

        return tokens;
    }

    BasicString& remove(size_t index, size_t count)
    {
        if (isEmpty())
            return *this;

        ASSERT(index < length());
        const size_t charactersToRemove = min(count, length() - index);
        memmove(&m_charBuffer[index], &m_charBuffer[index + charactersToRemove], length() - charactersToRemove - index);
        m_length -= charactersToRemove;
        return *this;
    }

    BasicString& insert(size_t index, const BasicString& str)
    {
        ASSERT(index <= m_length);

        // Append to the end
        if (index == m_length) {
            append(str);
            return *this;
        }

        if (m_capacity < length() + str.length() + 1)
            reserve(length() + str.length());

        // Insert in the middle
        memmove(&m_charBuffer[index + str.length()], &m_charBuffer[index], length() - index);
        m_length += str.length();
        memcpy(&m_charBuffer[index], str.m_charBuffer, str.length());
        return *this;
    }

    bool toDouble(double& value) const
    {
        char* endPtr = nullptr;
        const double doubleValue = strtod(m_charBuffer, &endPtr);

        if (endPtr == m_charBuffer)
            return false;

        if (*endPtr != 0)
            return false;

        value = doubleValue;
        return true;
    }

    bool toFloat(float& value) const
    {
        double doubleValue {};
        const bool retValue = toDouble(doubleValue);
        if (retValue)
            value = doubleValue;
        return retValue;
    }

    bool toInt(int32& value, const int32 base = 10) const
    {
        char* endPtr = nullptr;
        const long intValue = strtol(m_charBuffer, &endPtr, base);

        if (m_charBuffer == endPtr)
            return false;

        if (*endPtr != 0)
            return false;

        value = static_cast<int32>(intValue);
        return true;
    }

    BasicString& fromFloat(float value)
    {
        // FIXME: Implement precision.

        const int length = snprintf(nullptr, 0, "%f", value);
        if (length + 1 > m_capacity)
            reserve(length + 1);

        snprintf(m_charBuffer, length + 1, "%f", value);
        m_length = length;
        return *this;
    }

    BasicString& fromInt(int32 value)
    {
        const int length = snprintf(nullptr, 0, "%d", value);
        if (length + 1 > m_capacity)
            reserve(length + 1);

        snprintf(m_charBuffer, length + 1, "%d", value);
        m_length = length;
        return *this;
    }

#if 1
    friend std::ostream& operator<<(std::ostream& os, const BasicString& str)
    {
        if (!str.isEmpty()) {
            os << str.m_charBuffer;
        }
        return os;
    }
#endif

private:
    T* m_charBuffer { nullptr };
    size_t m_length { 0 };
    size_t m_capacity { 0 };
};

using String = BasicString<char>;

}
