cmake_minimum_required(VERSION 3.23)
project(OS)

enable_testing()

add_subdirectory(ADS)
add_subdirectory(Kernel)
add_subdirectory(ELFLoader)
add_subdirectory(Userland)
add_subdirectory(Tests)

