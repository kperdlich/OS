//
// Created by n3dry on 23.09.24.
//

#include "String.h"
#include <catch2/catch.hpp>

TEST_CASE("String: Basic operations work", "[String]")
{
    ADS::String str;
    CHECK(str.length() == 0);
    REQUIRE(str.isEmpty());

    const ADS::String test = "test";
    CHECK(test.length() == 4);
    CHECK_FALSE(test.isEmpty());
    REQUIRE(test != str);

    const ADS::String testCpy = test;
    REQUIRE(testCpy == test);
    REQUIRE(testCpy != str);

    const ADS::String splitTest = "test1 test2 test3";
    REQUIRE(splitTest == "test1 test2 test3");
    const auto splits = splitTest.split(' ');
    REQUIRE(splits.size() == 3);
    REQUIRE(splits[0] == "test1");
    REQUIRE(splits[1] == "test2");
    REQUIRE(splits[2] == "test3");

    ADS::String concatStr = "ABC";
    concatStr.append(" DEF");
    REQUIRE(concatStr == "ABC DEF");
    concatStr.appendChar('G');
    REQUIRE(concatStr == "ABC DEFG");
    const ADS::String additionalString = " HIJK";
    concatStr.append(additionalString);
    REQUIRE(concatStr == "ABC DEFG HIJK");
}

TEST_CASE("String: substring works", "[String]")
{
    const ADS::String str = "ABCDEF";
    REQUIRE(str.substring(0) == "ABCDEF");
    REQUIRE(str.substring(1, 3) == "BCDE");
    REQUIRE(str.substring(1, 20) == "BCDEF");
}

TEST_CASE("String: remove works", "[String]")
{
    ADS::String str = "ABCDEF";
    REQUIRE(str.remove(2, 1) == "ABDEF");
    REQUIRE(str.remove(0, 1) == "BDEF");
    REQUIRE(str.remove(1, 10) == "B");
    const ADS::String copy = str;
    REQUIRE(copy == "B");
}

TEST_CASE("String: insert works", "[String]")
{
    ADS::String str = "ABCDEF";
    REQUIRE(str.insert(0, "1") == "1ABCDEF");
    REQUIRE(str.insert(1, "23") == "123ABCDEF");
    REQUIRE(str.insert(1, "A") == "1A23ABCDEF");
    REQUIRE(str.insert(str.length(), "-XXX") == "1A23ABCDEF-XXX");
    REQUIRE(str.insert(3, "ZZZ") == "1A2ZZZ3ABCDEF-XXX");
}

TEST_CASE("String: toInt works", "[String]")
{
    ADS::String intStr = "1230";
    int intValue {};
    CHECK(intStr.toInt(intValue));
    REQUIRE(intValue == 1230);
}

TEST_CASE("String: toFloat works", "[String]")
{
    ADS::String floatStr = "1230.3";
    float floatValue {};
    CHECK(floatStr.toFloat(floatValue));
    REQUIRE(floatValue == 1230.3f);
}

TEST_CASE("String: toDouble works", "[String]")
{
    ADS::String doubleStr = "1230.30";
    double doubleValue {};
    CHECK(doubleStr.toDouble(doubleValue));
    REQUIRE(doubleValue == 1230.30);
}

TEST_CASE("String: fromFloat works", "[String]")
{
    ADS::String fromFloatValue = ADS::String::fromFloat(1230.30f);
    ASSERT(fromFloatValue.startsWith("1230.3"));
}

TEST_CASE("String: fromInt works", "[String]")
{
    ADS::String fromIntValue = ADS::String::fromInt(1230);
    ASSERT(fromIntValue == "1230");
}

TEST_CASE("String: startsWith works", "[String]")
{
    const ADS::String test = "test1";
    REQUIRE(test.startsWith("test"));
    REQUIRE_FALSE(test.startsWith("test123"));
    REQUIRE_FALSE(test.startsWith(""));
    REQUIRE(test.startsWith("test1"));
}

TEST_CASE("String: endsWith works", "[String]")
{
    const ADS::String test = "test1";
    REQUIRE(test.endsWith("test1"));
    REQUIRE(test.endsWith("t1"));
    REQUIRE_FALSE(test.endsWith("ttest1"));
    REQUIRE_FALSE(test.endsWith(""));
}
TEST_CASE("String: toUpper works", "[String]")
{
    ADS::String test = "aB!cD";
    REQUIRE(test.toUpper() == "AB!CD");
}

TEST_CASE("String: toLower works", "[String]")
{
    ADS::String test = "aB!cD";
    REQUIRE(test.toLower() == "ab!cd");
}

TEST_CASE("String: cStr works", "[String]")
{
    const ADS::String empty;
    REQUIRE(empty.cStr() == nullptr);

    const ADS::String test = "aB!cD";
    CHECK(test.cStr() != nullptr);
    REQUIRE(strcmp(test.cStr(), "aB!cD") == 0);
}
