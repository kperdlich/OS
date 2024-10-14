cmake_minimum_required(VERSION 3.23)
project(OS)

option(BUILD_KERNEL "Build the kernel" ON)

enable_testing()

add_subdirectory(ADS)
if(BUILD_KERNEL)
    add_subdirectory(Kernel)
endif()
add_subdirectory(ELFLoader)
add_subdirectory(Userland)
add_subdirectory(Tests)

