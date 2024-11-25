//
// Created by n3dry on 23.09.24.
//

#include "Vector.h"
#include <catch2/catch.hpp>

TEST_CASE("Vector: basic operations work", "[Vector]")
{
    ADS::Vector<int> test;
    test.pushBack(10);
    test.pushBack(20);
    test.pushBack(30);
    test.pushBack(40);
    REQUIRE(test.size() == 4);
    test.popBack();
    test.popBack();
    REQUIRE(test.size() == 2);
    test.pushBack(24);
    REQUIRE(test.size() == 3);

    REQUIRE(test.at(0) == 10);
    REQUIRE(test.at(1) == 20);
    REQUIRE(test.at(2) == 24);

    for (auto it : test) {
        INFO(it);
    }

    for (auto it = test.rbegin(); it != test.rend(); ++it) {
        INFO(*it);
    }

    ADS::Vector<int> vector = { 10, 20, 30, 60 };
    for (auto it : vector) {
        INFO(it);
    }
}

TEST_CASE("Vector: find works", "[Vector]")
{
    ADS::Vector<int> test;
    test.pushBack(10);
    test.pushBack(20);
    test.pushBack(30);
    test.pushBack(40);

    auto middle = test.find(20);
    CHECK(middle != test.end());
    REQUIRE(*middle == 20);

    auto begin = test.find(10);
    CHECK(begin != test.end());
    REQUIRE(*begin == 10);

    auto end = test.find(40);
    CHECK(end != test.end());
    REQUIRE(*end == 40);

    auto notFound = test.find(100);
    REQUIRE(notFound == test.end());
}

TEST_CASE("Vector: remove works", "[Vector]")
{
    ADS::Vector<int> test;
    test.pushBack(10);
    test.pushBack(20);
    test.pushBack(30);
    test.pushBack(40);

    static constexpr const int valueToRemove = 20;
    auto it = test.find(valueToRemove);
    CHECK(it != test.end());

    test.remove(it);
    CHECK(test.size() == 3);
    CHECK(test.at(0) == 10);
    CHECK(test.at(1) == 30);
    CHECK(test.at(2) == 40);
    REQUIRE(test.find(valueToRemove) == test.end());
}

TEST_CASE("Vector: removeAll works", "[Vector]")
{
    ADS::Vector<int> test;
    test.pushBack(10);
    test.pushBack(20);
    test.pushBack(20);
    test.pushBack(30);
    test.pushBack(40);

    static constexpr const int valueToRemove = 20;

    ADS::size_t removedElements = test.removeAll(valueToRemove);
    CHECK(test.size() == 3);
    CHECK(test.at(0) == 10);
    CHECK(test.at(1) == 30);
    CHECK(test.at(2) == 40);
    CHECK(test.find(valueToRemove) == test.end());
    REQUIRE(removedElements == 2);
}

class TestClass {
public:
    inline static ADS::size_t moveCtorCounter = 0;
    inline static ADS::size_t destructorCounter = 0;

    static void ResetCounter()
    {
        moveCtorCounter = { 0 };
        destructorCounter = { 0 };
    }

    TestClass(int value)
        : m_value { value }
    {
    }

    ~TestClass()
    {
        ++destructorCounter;
    }

    TestClass(TestClass&& other)
        : m_value(ADS::move(other.m_value))
    {
        other.m_value = {};
        ++moveCtorCounter;
    }
    int m_value {};
};

TEST_CASE("Vector: reallocation calls move and destructor correctly", "[Vector]")
{
    TestClass::ResetCounter();
    ADS::Vector<TestClass> test;
    test.pushBack({ 1 });
    test.pushBack({ 2 });
    test.pushBack({ 3 });
    test.pushBack({ 4 });
    test.pushBack({ 5 });

    // Pushback with 5 tmp objects (destructor) + move
    REQUIRE(TestClass::moveCtorCounter == 5);
    REQUIRE(TestClass::destructorCounter == 5);

    // One more destructor + move call
    test.pushBack({ 6 }); // should trigger reallocation

    REQUIRE(test.at(0).m_value == 1);
    REQUIRE(test.at(1).m_value == 2);
    REQUIRE(test.at(2).m_value == 3);
    REQUIRE(test.at(3).m_value == 4);
    REQUIRE(test.at(4).m_value == 5);
    REQUIRE(test.at(5).m_value == 6);

    // 6 objects in general and 5 should be moved + destructor call during reallocation
    REQUIRE(TestClass::moveCtorCounter == 6 + 5);
    REQUIRE(TestClass::destructorCounter == 6 + 5);
}

TEST_CASE("Vector: remove calls move and destructor correctly when shifting elements", "[Vector]")
{
    TestClass::ResetCounter();
    ADS::Vector<TestClass> test;
    test.pushBack({ 1 });
    test.pushBack({ 2 });
    test.pushBack({ 3 });

    // Pushback with 3 tmp objects (destructor) + move
    REQUIRE(TestClass::moveCtorCounter == 3);
    REQUIRE(TestClass::destructorCounter == 3);

    auto it = test.begin();
    test.remove(++it);

    // One object removed and one shifted (destructor + move)
    REQUIRE(TestClass::moveCtorCounter == 3 + 1);
    REQUIRE(TestClass::destructorCounter == 3 + 2);
}