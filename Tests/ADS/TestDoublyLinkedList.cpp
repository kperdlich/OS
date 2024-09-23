//
// Created by n3dry on 23.09.24.
//

#include "DoublyLinkedList.h"
#include <catch2/catch.hpp>

TEST_CASE("DoublyLinkedList: basic operations work", "[DoublyLinkedList]")
{
    ADS::DoublyLinkedList<int> emptyList;
    REQUIRE(emptyList.isEmpty());
    for (auto item : emptyList) {
        REQUIRE(false);
    }

    ADS::DoublyLinkedList<int> filledList;
    int test = 20;
    filledList.add(10);
    filledList.add(30);
    filledList.add(ADS::move(test));
    REQUIRE_FALSE(filledList.isEmpty());
    REQUIRE(filledList.size() == 3);
    for (auto item : filledList) {
        INFO(item);
    }
    auto iterator = filledList.find(30);
    REQUIRE(iterator != filledList.end());
    REQUIRE(*iterator == 30);
    REQUIRE(*(--iterator) == 10);
    REQUIRE(filledList.remove(iterator));
    REQUIRE(filledList.size() == 2);
    REQUIRE(!filledList.remove(filledList.end()));
    REQUIRE(filledList.remove(filledList.begin()));
    REQUIRE(filledList.size() == 1);
    REQUIRE(filledList.find(20) != filledList.end());
    for (auto item : filledList) {
        REQUIRE(item == 20);
    }
    filledList.clear();
    REQUIRE(filledList.size() == 0);
    REQUIRE(filledList.isEmpty());
}