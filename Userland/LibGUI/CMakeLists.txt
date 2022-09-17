cmake_minimum_required(VERSION 3.23)
project(LibGUI)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(LibGUI main.cpp)
target_link_libraries(LibGUI ${SDL2_LIBRARIES})
