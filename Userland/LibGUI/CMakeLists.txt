cmake_minimum_required(VERSION 3.23)
project(LibGUI)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(../../ADS)

add_executable(LibGUI main.cpp Widget.cpp Widget.h Window.cpp Window.h Painter.cpp Painter.h Rect.h Bitmap.cpp Bitmap.h Color.cpp Color.h Size.h WindowManager.cpp WindowManager.h Button.cpp Button.h CObject.cpp CObject.h Screen.cpp Screen.h DefaultFont8x10.h ASCIIFont.cpp ASCIIFont.h ApplicationSDL.cpp Application.h Point.h CharacterBitmap.cpp CharacterBitmap.h TextBox.h TextBox.cpp Event.h TimerManager.h TimerManagerSDL.cpp FocusReason.h Label.cpp Label.h Alignment.h)
target_link_libraries(LibGUI ${SDL2_LIBRARIES})
