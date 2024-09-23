//
// Created by n3dry on 23.09.24.
//

#include "SinglyLinkedList.h"
#include <catch2/catch.hpp>

TEST_CASE("SinglyLinkedList: basis operations work", "[SinglyLinkedList]")
{
    ADS::SinglyLinkedList<int> emptyList;
    REQUIRE(emptyList.isEmpty());
    for (auto item : emptyList) {
        REQUIRE(false);
    }

    ADS::SinglyLinkedList<int> filledList;
    int test = 20;
    filledList.add(10);
    filledList.add(30);
    filledList.add(ADS::move(test));
    REQUIRE_FALSE(filledList.isEmpty());
    for (auto item : filledList) {
        INFO(item);
    }

    auto iterator = filledList.find(30);
    REQUIRE(iterator != filledList.end());
    REQUIRE(*iterator == 30);
    REQUIRE(filledList.remove(iterator));
    REQUIRE(!filledList.remove(filledList.end()));
    REQUIRE(filledList.remove(filledList.begin()));
    REQUIRE(filledList.find(20) != filledList.end());
    for (auto item : filledList) {
        REQUIRE(item == 20);
    }
    filledList.clear();
    REQUIRE(filledList.isEmpty());
}