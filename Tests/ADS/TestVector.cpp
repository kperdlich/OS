//
// Created by n3dry on 23.09.24.
//

#include "Vector.h"
#include <catch2/catch.hpp>

TEST_CASE("Vector: basic operations work", "[Vector]")
{
    TEST::Vector<int> test;
    test.pushBack(10);
    test.pushBack(20);
    test.pushBack(30);
    test.pushBack(40);
    REQUIRE(test.size() == 4);
    test.popBack();
    test.popBack();
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

    TEST::Vector<int> vector = { 10, 20, 30, 60 };
    for (auto it : vector) {
        INFO(it);
    }
}
