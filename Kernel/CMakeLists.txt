project(Kernel)

enable_language(ASM)

set(TOOLCHAIN_BIN_PATH ${CMAKE_SOURCE_DIR}/Toolchain/Local/i686/bin)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN_PATH}/i686-elf-g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_BIN_PATH}/i686-elf-as)
set(CMAKE_LINKER ${TOOLCHAIN_BIN_PATH}/i686-elf-ld)

if (${CMAKE_GENERATOR} STREQUAL "Ninja")
    # By default ninja adds `-MF` as an option which doesn't seem to be available in i686-elf-as.
    set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> -c <SOURCE> -o <OBJECT>")
endif ()

set(CMAKE_CXX_FLAGS "-m32 -ffreestanding -fno-stack-protector -fno-pic -fno-exceptions -fno-rtti -nostdlib -Wextra -Wall -Wundef -Werror")

set(SOURCES
        boot.s
        Kernel.cpp
        VGA.h
        VGA.cpp
        StdLib.h
        StdLib.cpp
)

set(LINKER_SCRIPT ${PROJECT_SOURCE_DIR}/linker.ld)
set(KERNEL_BIN "Kernel-i386.bin")

add_executable(Kernel ${SOURCES})

set_target_properties(Kernel PROPERTIES LINK_FLAGS "-T ${LINKER_SCRIPT} -ffreestanding -nostdlib")
set_target_properties(Kernel PROPERTIES OUTPUT_NAME ${KERNEL_BIN})

target_link_libraries(Kernel PRIVATE ADS)

target_compile_definitions(Kernel PUBLIC KERNEL)

add_custom_target(BuildAndRunKernel
        DEPENDS Kernel
        COMMAND qemu-system-i386 -kernel ${PROJECT_BINARY_DIR}/${KERNEL_BIN}
        COMMENT "Running the kernel in QEMU..."
)