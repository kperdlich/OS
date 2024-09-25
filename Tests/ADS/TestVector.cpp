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
