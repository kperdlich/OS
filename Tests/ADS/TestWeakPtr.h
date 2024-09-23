//
// Created by n3dry on 23.09.24.
//

#include "Move.h"
#include "RefPtr.h"
#include <catch2/catch.hpp>

TEST_CASE("WeakPtr: basic operations work", "[WeakPtr]")
{
    ADS::WeakPtr<int> weakPtrDefault;
    REQUIRE(weakPtrDefault.refCount() == 0);
    REQUIRE(weakPtrDefault.isNull());

    ADS::WeakPtr<int> weakPtrNull = nullptr;
    REQUIRE(weakPtrNull.refCount() == 0);
    REQUIRE(weakPtrNull.isNull());

    ADS::RefPtr<int> refPtrEmpty = nullptr;
    ADS::WeakPtr<int> weakPtrEmptyRefPtr = refPtrEmpty;
    REQUIRE(weakPtrEmptyRefPtr.refCount() == 0);
    REQUIRE(weakPtrEmptyRefPtr.isNull());

    ADS::RefPtr<int> refPtrInt = ADS::makeRef<int>(30);
    ADS::WeakPtr<int> weakPtrInt = refPtrInt;
    REQUIRE(weakPtrInt.refCount() == 1);
    REQUIRE(!weakPtrInt.isNull());
    {
        if (ADS::RefPtr scopedRefPtrInt = weakPtrInt.lock()) {
            REQUIRE(scopedRefPtrInt.refCount() == 2);
            REQUIRE(refPtrInt.refCount() == 2);
            REQUIRE(weakPtrInt.refCount() == 2);
            REQUIRE(*scopedRefPtrInt == 30);
        } else {
            REQUIRE(false);
        }
        REQUIRE(refPtrInt.refCount() == 1);
    }

    REQUIRE(weakPtrInt.refCount() == 1);
    REQUIRE(!weakPtrInt.isNull());
    weakPtrInt.clear();
    REQUIRE(weakPtrInt.isNull());
    REQUIRE(weakPtrInt.refCount() == 0);

    ADS::RefPtr emptyRefPtr = weakPtrInt.lock();
    REQUIRE(emptyRefPtr == nullptr);
}