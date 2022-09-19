cmake_minimum_required(VERSION 3.23)
project(LibGUI)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(LibGUI main.cpp Widget.cpp Widget.h Window.cpp Window.h Painter.cpp Painter.h Rect.h Bitmap.cpp Bitmap.h Types.h Color.cpp Color.h Size.h)
target_link_libraries(LibGUI ${SDL2_LIBRARIES})
