//
// Created by n3dry on 07.08.24.
//

#pragma once

#include "Move.h"

namespace ADS {

template<typename T>
class DoublyLinkedList {
private:
    struct Node {
        explicit Node(T&& value)
            : m_data(value)
        {
        }

        T m_data;
        Node* m_next { nullptr };
        Node* m_prev { nullptr };
    };

public:
    class Iterator {
        friend class DoublyLinkedList;

    public:
        Iterator(Node* node)
            : m_node(node)
        {
        }

        T& operator*()
        {
            ASSERT(m_node != nullptr);
            return m_node->m_data;
        }

        Iterator& operator++()
        {
            m_node = m_node->m_next;
            return *this;
        }

        Iterator& operator--()
        {
            m_node = m_node->m_prev;
            return *this;
        }

        bool operator==(const Iterator& other) const
        {
            return m_node == other.m_node;
        }

        bool operator!=(const Iterator& other) const
        {
            return m_node != other.m_node;
        }

    private:
        Node* m_node { nullptr };
    };

    class ConstIterator {
        friend class DoublyLinkedList;

    public:
        ConstIterator(Node* node)
            : m_node(node)
        {
        }

        const T& operator*()
        {
            ASSERT(m_node != nullptr);
            return m_node->m_data;
        }

        ConstIterator& operator++()
        {
            m_node = m_node->m_next;
            return *this;
        }

        ConstIterator& operator--()
        {
            m_node = m_node->m_prev;
            return *this;
        }

        bool operator==(const ConstIterator& other)
        {
            return m_node == other.m_node;
        }

        bool operator!=(const ConstIterator& other)
        {
            return m_node != other.m_node;
        }

    private:
        Node* m_node { nullptr };
    };

public:
    DoublyLinkedList() = default;
    ~DoublyLinkedList()
    {
        clear();
    }

    DoublyLinkedList(const DoublyLinkedList& other) = delete;
    DoublyLinkedList(DoublyLinkedList&& other)
        : m_head(other.m_head)
        , m_tail(other.m_tail)
    {
        other.m_head = nullptr;
        other.m_tail = nullptr;
    }

    DoublyLinkedList& operator=(const DoublyLinkedList& other) = delete;
    DoublyLinkedList& operator=(DoublyLinkedList&& other)
    {
        if (this != &other) {
            clear();
            m_head = other.m_head;
            m_tail = other.m_tail;
            other.m_head = nullptr;
            other.m_tail = nullptr;
        }
    }

    void clear()
    {
        for (Node* node = m_head; node;) {
            Node* const next = node->m_next;
            delete node;
            node = next;
        }

        m_head = nullptr;
        m_tail = nullptr;
    }

    void add(T&& value)
    {
        Node* const node = new Node(ADS::move(value));
        if (!m_head) {
            m_head = node;
            m_tail = node;
        } else {
            m_tail->m_next = node;
            node->m_prev = m_tail;
            m_tail = node;
        }
    }

    bool remove(const Iterator& iterator)
    {
        if (iterator == end() || m_head == nullptr)
            return false;

        if (Iterator(m_head) == iterator) {
            Node* headToDelete = m_head;
            Node* newHead = m_head->m_next;
            m_head = newHead;
            if (m_head)
                m_head->m_prev = nullptr;
            else
                m_tail = nullptr;
            delete headToDelete;
            headToDelete = nullptr;
            return true;
        }

        Node* nodeToDelete = iterator.m_node;
        Node* prev = nodeToDelete->m_prev;
        Node* next = nodeToDelete->m_next;
        prev->m_next = next;
        if (next)
            next->m_prev = prev;
        else
            m_tail = prev;
        delete nodeToDelete;
        nodeToDelete = nullptr;
        return true;
    }

    Iterator find(const T& value)
    {
        for (Node* node = m_head; node; node = node->m_next) {
            if (node->m_data == value)
                return Iterator(node);
        }

        return end();
    }

    bool contains(const T& value) const
    {
        for (Node* node = m_head; node; node = node->m_next) {
            if (node->m_data == value)
                return true;
        }

        return false;
    }

    bool isEmpty() const
    {
        return m_head == nullptr;
    }

    Iterator begin()
    {
        return Iterator(m_head);
    }

    Iterator end()
    {
        return Iterator(nullptr);
    }

    ConstIterator begin() const
    {
        return ConstIterator(m_head);
    }

    ConstIterator end() const
    {
        return ConstIterator(nullptr);
    }

private:
    Node* m_head { nullptr };
    Node* m_tail { nullptr };
};

}
