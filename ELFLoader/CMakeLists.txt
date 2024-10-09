project(ELFLoader)

set(CMAKE_CXX_STANDARD 20)

add_executable(ELFTestProgram
        test.cpp
)

add_executable(ELFLoader
        main.cpp
        ELFLoader.cpp
        ELFLoader.h
)

target_link_libraries(ELFLoader PRIVATE ADS)