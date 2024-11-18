//
// Created by n3dry on 23.09.24.
//

#include "Move.h"
#include "RefPtr.h"
#include <catch2/catch.hpp>

TEST_CASE("RefPtr: basic operations work", "[RefPtr]")
{
    ADS::RefPtr<int> refPtrDefault = nullptr;
    ADS::RefPtr<int> refPtr = nullptr;
    REQUIRE(refPtr.refCount() == 0);
    REQUIRE(refPtrDefault.refCount() == 0);

    ADS::RefPtr<int> refPtrInt = ADS::makeRef<int>(2);
    REQUIRE(*refPtrInt == 2);
    REQUIRE(refPtrInt.refCount() == 1);
    {
        ADS::RefPtr<int> refPtrIntCopy = refPtrInt;
        REQUIRE(*refPtrIntCopy == 2);
        REQUIRE(refPtrIntCopy.refCount() == 2);
        REQUIRE(refPtrInt == refPtrIntCopy);
    }
    REQUIRE(refPtrInt.refCount() == 1);
}
