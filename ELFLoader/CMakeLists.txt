project(ELFLoader)

set(CMAKE_CXX_STANDARD 20)

add_executable(ELFTestProgram
        test.cpp
)

set_target_properties(ELFTestProgram PROPERTIES COMPILE_OPTIONS "-m32" LINK_FLAGS "-m32")

add_executable(ELFLoader
        main.cpp
        ELFLoader.cpp
        ELFLoader.h
)

target_link_libraries(ELFLoader PRIVATE ADS)