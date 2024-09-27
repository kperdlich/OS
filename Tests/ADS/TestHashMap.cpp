//
// Created by n3dry on 23.09.24.
//

#include "HashMap.h"
#include <catch2/catch.hpp>
#include <iostream>

TEST_CASE("HashMap: insert and remove works", "[HashMap]")
{
    ADS::HashMap<int, int> map;
    for (auto& entry : map)
        REQUIRE_FALSE(false);

    REQUIRE(map.isEmpty());
    map.set(1, 10);
    CHECK_FALSE(map.isEmpty());
    CHECK(map.contains(1));
    CHECK_FALSE(map.contains(2));
    REQUIRE(map.getValueOrDefault(1, -1) == 10);
    REQUIRE(map.getValueOrDefault(2, -1) == -1);

    {
        int test;
        REQUIRE(map.tryGetValue(1, test));
        REQUIRE(test == 10);
        REQUIRE_FALSE(map.tryGetValue(2, test));
    }

    {
        map.set(10, 20);
        CHECK(map.contains(10));
        REQUIRE(map.size() == 2);
        REQUIRE(map.remove(10) == 1);
        REQUIRE(map.remove(100) == 0);
        REQUIRE(map.size() == 1);
    }
}

TEST_CASE("HashMap: rehash works", "[HashMap]")
{
    ADS::HashMap<int, int> map;
    map.set(1, 20);
    map.set(2, 20);
    map.set(3, 20);
    map.set(4, 20);
    map.set(5, 20);
    map.set(6, 20);

    REQUIRE(map.contains(1));
    REQUIRE(map.contains(2));
    REQUIRE(map.contains(3));
    REQUIRE(map.contains(4));
    REQUIRE(map.contains(5));
    REQUIRE(map.contains(6));

    INFO("HashMap iterator:");
    for (auto& entry : map)
        INFO("[" << entry.key << " -> " << entry.value << "]");

    const ADS::HashMap<int, int> constMap = ADS::move(map);
    INFO("HashMap ConstIterator:");
    for (const auto& entry : constMap)
        INFO("[" << entry.key << " -> " << entry.value << "]");

    INFO("HashMap: " << map.toString());
}
