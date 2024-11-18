find_package(Catch2 REQUIRED)

add_executable(ADSTest
        TestHashMap.cpp
        TestVector.cpp
        TestString.cpp
        TestFunction.cpp
        TestOwnPtr.cpp
        TestRefPtr.cpp
        TestWeakPtr.cpp
        TestSinglyLinkedList.cpp
        TestDoublyLinkedList.cpp
)

target_link_libraries(ADSTest PRIVATE ADS Catch2::Catch2WithMain)

include(CTest)
include(Catch)

catch_discover_tests(ADSTest)