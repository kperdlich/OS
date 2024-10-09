project(ELFLoader)

set(CMAKE_CXX_STANDARD 20)

add_custom_target(ELFTestProgram
        COMMAND ${CMAKE_CXX_COMPILER} -c ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
        COMMENT "Compiling test.cpp into object file test.o"
)

add_executable(ELFLoader
        main.cpp
        ELFLoader.cpp
        ELFLoader.h
)

target_link_libraries(ELFLoader PRIVATE ADS)

add_dependencies(ELFLoader ELFTestProgram)
