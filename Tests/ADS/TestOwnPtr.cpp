//
// Created by n3dry on 23.09.24.
//

#include "Move.h"
#include "OwnPtr.h"
#include <catch2/catch.hpp>

TEST_CASE("OwnPtr: basic operations work", "[OwnPtr]")
{
    ADS::OwnPtr<int> ownPtrDefault;
    REQUIRE(ownPtrDefault.ptr() == nullptr);
    ADS::OwnPtr<int> ownPtrEmpty = nullptr;
    REQUIRE(ownPtrEmpty.ptr() == nullptr);

    ADS::OwnPtr<int> ownPtr = ADS::makeOwn<int>(13);
    REQUIRE(*ownPtr == 13);

    ownPtrEmpty = ADS::move(ownPtr);
    REQUIRE(*ownPtrEmpty == 13);
    REQUIRE(ownPtr.ptr() == nullptr);

    ownPtrEmpty.clear();
    REQUIRE(ownPtrEmpty.ptr() == nullptr);
}
