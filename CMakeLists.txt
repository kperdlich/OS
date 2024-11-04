cmake_minimum_required(VERSION 3.23)
project(OS)

option(BUILD_KERNEL "Build the kernel" ON)
option(KERNEL_RUN_DIVIDE_BY_ZERO_TEST "Run divide-by-zero exception in Kernel" OFF)
option(KERNEL_TRACE_HEAP_ALLOC "Log kmalloc and kfree" OFF)

enable_testing()

add_subdirectory(ADS)
if (BUILD_KERNEL)
    add_subdirectory(Kernel)
endif ()
add_subdirectory(ELFLoader)
add_subdirectory(Userland)
add_subdirectory(Tests)

message(STATUS "**************** BUILD INFO ****************")
message(STATUS "BUILD_KERNEL: \"${BUILD_KERNEL}\"")
message(STATUS "KERNEL_RUN_DIVIDE_BY_ZERO_TEST: \"${KERNEL_RUN_DIVIDE_BY_ZERO_TEST}\"")


