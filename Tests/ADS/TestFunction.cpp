//
// Created by n3dry on 23.09.24.
//

#include "Function.h"
#include <catch2/catch.hpp>

TEST_CASE("Function: basic lambda works", "[Function]")
{
    constexpr const int expectedIntValue = 10;

    ADS::Function<int(int)> fx = [expectedIntValue](int x) -> int {
        ASSERT(x == expectedIntValue);
        return x;
    };
    REQUIRE(fx(expectedIntValue) == expectedIntValue);

    ADS::Function<int(int)> movedFx = ADS::move(fx);
    REQUIRE(movedFx(expectedIntValue) == expectedIntValue);
}