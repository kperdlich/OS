//
// Created by n3dry on 09.08.24.
//

#pragma once

#include "DoublyLinkedList.h"
#include "Hash.h"
#include "Types.h"
#include <string>

namespace ADS {

template<typename K, typename V, typename HashFunc = Hash<K>>
class HashMap {
private:
    static const constexpr size_t DefaultBuckets = 5;

    struct Entry {
        K key;
        V value;
    };

    struct Bucket {
        DoublyLinkedList<Entry> list;
    };

public:
    class Iterator {
    public:
        enum class LookForFirstEntry {
            Yes,
            No,
        };

        Iterator(HashMap& hashMap, const DoublyLinkedList<Entry>::Iterator& startIterator, LookForFirstEntry lookForFirstEntry, size_t bucketIndex = 0)
            : m_hashMap(hashMap)
            , m_bucketIterator(startIterator)
            , m_bucketIndex(bucketIndex)
        {
            const bool startIteratorHasNoElement = startIterator == startIterator.end();
            if (startIteratorHasNoElement && lookForFirstEntry == LookForFirstEntry::Yes) {
                lookForNextEntry();
            }
        }

        HashMap::Entry& operator*()
        {
            return m_bucketIterator.operator*();
        }

        const Entry* operator->() const
        {
            return m_bucketIterator.operator->();
        }

        HashMap::Entry* operator->()
        {
            return m_bucketIterator.operator->();
        }

        Iterator& operator++()
        {
            lookForNextEntry();
            return *this;
        }

        bool operator==(const Iterator& other) const
        {
            return &m_hashMap == &other.m_hashMap
                && m_bucketIterator == other.m_bucketIterator
                && m_bucketIndex == other.m_bucketIndex;
        }

        bool operator!=(const Iterator& other) const
        {
            return !operator==(other);
        }

    private:
        // Walks the bucket list and underlying linked list to find next entry.
        void lookForNextEntry()
        {
            while (true) {
                Bucket& currentBucket = m_hashMap.m_buckets[m_bucketIndex];
                if (m_bucketIterator == currentBucket.list.end()) {
                    if (m_bucketIndex + 1 >= m_hashMap.m_bucketCount)
                        return;

                    // Walk buckets
                    ++m_bucketIndex;
                    Bucket& nextBucket = m_hashMap.m_buckets[m_bucketIndex];
                    m_bucketIterator = nextBucket.list.begin();
                } else {
                    // Walk bucket linked list
                    ++m_bucketIterator;
                }

                if (m_bucketIterator != m_hashMap.m_buckets[m_bucketIndex].list.end())
                    return;
            }
        }

    private:
        HashMap& m_hashMap;
        typename DoublyLinkedList<Entry>::Iterator m_bucketIterator;
        size_t m_bucketIndex { 0 };
    };

    class ConstIterator {
    public:
        enum class LookForFirstEntry {
            Yes,
            No,
        };

        ConstIterator(const HashMap& hashMap, const DoublyLinkedList<Entry>::ConstIterator& startIterator, LookForFirstEntry lookForFirstEntry, size_t bucketIndex = 0)
            : m_hashMap(hashMap)
            , m_bucketIterator(startIterator)
            , m_bucketIndex(bucketIndex)
        {
            const bool startIteratorHasNoElement = startIterator == startIterator.end();
            if (startIteratorHasNoElement && lookForFirstEntry == LookForFirstEntry::Yes) {
                lookForNextEntry();
            }
        }

        const HashMap::Entry& operator*() const
        {
            return m_bucketIterator.operator*();
        }

        const HashMap::Entry* operator->() const
        {
            return m_bucketIterator.operator->();
        }

        ConstIterator& operator++()
        {
            lookForNextEntry();
            return *this;
        }

        bool operator==(const ConstIterator& other) const
        {
            return &m_hashMap == &other.m_hashMap
                && m_bucketIterator == other.m_bucketIterator
                && m_bucketIndex == other.m_bucketIndex;
        }

        bool operator!=(const ConstIterator& other) const
        {
            return !operator==(other);
        }

    private:
        // Walks the bucket list and underlying linked list to find next entry.
        void lookForNextEntry()
        {
            while (true) {
                const Bucket& currentBucket = m_hashMap.m_buckets[m_bucketIndex];
                if (m_bucketIterator == currentBucket.list.end()) {
                    if (m_bucketIndex + 1 >= m_hashMap.m_bucketCount)
                        return;

                    // Walk buckets
                    ++m_bucketIndex;
                    const Bucket& nextBucket = m_hashMap.m_buckets[m_bucketIndex];
                    m_bucketIterator = nextBucket.list.begin();
                } else {
                    // Walk bucket linked list
                    ++m_bucketIterator;
                }

                const Bucket& newBucket = m_hashMap.m_buckets[m_bucketIndex];
                if (m_bucketIterator != newBucket.list.end())
                    return;
            }
        }

    private:
        const HashMap& m_hashMap;
        typename DoublyLinkedList<Entry>::ConstIterator m_bucketIterator;
        size_t m_bucketIndex { 0 };
    };

    HashMap() = default;
    HashMap(const HashMap& other) = delete;
    HashMap& operator=(const HashMap& other) = delete;

    HashMap(HashMap&& other)
    {
        if (this != &other) {
            m_elementCount = other.m_elementCount;
            m_bucketCount = other.m_bucketCount;
            m_hashFunc = other.m_hashFunc;

            other.m_buckets = nullptr;
            other.m_elementCount = 0;
            other.m_bucketCount = 0;
        }
    }

    HashMap& operator=(HashMap&& other)
    {
        if (this != &other) {
            clear();
            m_buckets = other.m_buckets;
            m_elementCount = other.m_elementCount;
            m_bucketCount = other.m_bucketCount;
            m_hashFunc = other.m_hashFunc;

            other.m_buckets = nullptr;
            other.m_elementCount = 0;
            other.m_bucketCount = 0;
        }
    }

    ~HashMap()
    {
        clear();
    }

    void set(K key, V value)
    {
        if (!m_buckets) {
            rehash(DefaultBuckets);
        }

        // Try to find key.
        const size_t bucketIndex = getIndexFor(key);
        Bucket& bucket = m_buckets[bucketIndex];
        for (Entry& element : bucket.list) {
            if (element.key == key) {
                element.value = ADS::move(value);
                return;
            }
        }

        // Key not found - add to the end of the bucket list.
        bucket.list.add(Entry { ADS::move(key), ADS::move(value) });
        ++m_elementCount;

        if (shouldRehash())
            rehash(m_bucketCount * 2);
    }

    bool contains(const K& key) const
    {
        const size_t bucketIndex = getIndexFor(key);
        Bucket& bucket = m_buckets[bucketIndex];
        for (Entry& element : bucket.list) {
            if (element.key == key) {
                return true;
            }
        }
        return false;
    }

    bool tryGetValue(const K& key, V& value) const
    {
        const size_t bucketIndex = getIndexFor(key);
        Bucket& bucket = m_buckets[bucketIndex];
        for (Entry& element : bucket.list) {
            if (element.key == key) {
                value = element.value;
                return true;
            }
        }
        return false;
    }

    V getValueOrDefault(const K& key, const V& defaultValue = V()) const
    {
        const size_t bucketIndex = getIndexFor(key);
        Bucket& bucket = m_buckets[bucketIndex];
        for (Entry& element : bucket.list) {
            if (element.key == key) {
                return element.value;
            }
        }
        return defaultValue;
    }

    size_t remove(const K& key)
    {
        const size_t bucketIndex = getIndexFor(key);
        Bucket& bucket = m_buckets[bucketIndex];
        const size_t removedElements = bucket.list.removeIf([&key](const DoublyLinkedList<Entry>::Iterator& it) {
            return it->key == key;
        });
        m_elementCount -= removedElements;
        return removedElements;
    }

    void clear()
    {
        if (!m_buckets)
            return;

        delete[] m_buckets;
        m_buckets = nullptr;
        m_elementCount = 0;
        m_bucketCount = 0;
    }

    int size() const { return m_elementCount; }
    int isEmpty() const { return size() == 0; }

    Iterator begin()
    {
        if (!m_buckets)
            return end();
        ASSERT(m_bucketCount > 0);
        return Iterator(*this, m_buckets[0].list.begin(), Iterator::LookForFirstEntry::Yes);
    }

    Iterator end()
    {
        return Iterator(*this, nullptr, Iterator::LookForFirstEntry::No, m_bucketCount - 1);
    }

    ConstIterator begin() const
    {
        if (!m_buckets)
            return end();
        ASSERT(m_bucketCount > 0);
        const DoublyLinkedList<Entry>& beginBucket =  m_buckets[0].list;
        return ConstIterator(*this, beginBucket.begin(), ConstIterator::LookForFirstEntry::Yes);
    }

    ConstIterator end() const
    {
        return ConstIterator(*this, nullptr, ConstIterator::LookForFirstEntry::No, m_bucketCount - 1);
    }

#if 1
    std::string toString() const
    {
        std::string str = "[";
        for (size_t i = 0; i < m_bucketCount; ++i) {
            const Bucket& bucket = m_buckets[i];
            for (const Entry& entry : bucket.list) {
                str += "{" + std::to_string(entry.key) + "->" + std::to_string(entry.value) + "},";
            }
        }

        if (str.back() == ',')
            str.pop_back();

        str += "]";
        return str;
    }
#endif

private:
    bool shouldRehash() const
    {
        const float factor = static_cast<float>(m_elementCount) / static_cast<float>(m_bucketCount);
        return factor > 0.75f;
    }

    void rehash(size_t newBucketCapacity)
    {
        const size_t oldBucketSize = m_bucketCount;
        const size_t oldElementCount = m_elementCount;
        const Bucket* oldBuckets = m_buckets;

        m_bucketCount = newBucketCapacity;
        m_elementCount = 0;
        m_buckets = new Bucket[m_bucketCount];

#if 1
        std::cout << "HashMap::rehash: From " << oldBucketSize << " buckets to " << m_elementCount << std::endl;
#endif

        for (size_t i = 0; i < oldBucketSize; ++i) {
            const Bucket& oldBucket = oldBuckets[i];
            for (const Entry& oldBucketElement : oldBucket.list) {
                set(ADS::move(oldBucketElement.key), ADS::move(oldBucketElement.value));
            }
        }

        delete[] oldBuckets;
        oldBuckets = nullptr;
    }

    size_t getIndexFor(const K& key) const
    {
        const uint32 hash = m_hashFunc(key);
        return hash % m_bucketCount;
    }

private:
    Bucket* m_buckets { nullptr };
    HashFunc m_hashFunc;
    size_t m_elementCount { 0 };
    size_t m_bucketCount { 0 };
};

}