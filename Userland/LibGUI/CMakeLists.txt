cmake_minimum_required(VERSION 3.23)
project(LibGUI)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(../../ADS)

add_executable(LibGUI main.cpp Widget.cpp Widget.h Window.cpp Window.h Painter.cpp Painter.h Rect.h Bitmap.cpp Bitmap.h ../../ADS/Types.h Color.cpp Color.h Size.h WindowStack.cpp WindowStack.h Button.cpp Button.h)
target_link_libraries(LibGUI ${SDL2_LIBRARIES})
