project(Kernel)

set(TOOLCHAIN_BIN_PATH ${CMAKE_SOURCE_DIR}/Toolchain/Local/i686/bin)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN_PATH}/i686-elf-g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_BIN_PATH}/i686-elf-as)
set(CMAKE_LINKER ${TOOLCHAIN_BIN_PATH}/i686-elf-ld)

set(CMAKE_CXX_FLAGS -m32 -ffreestanding -fno-stack-protector -fno-pic -fno-exceptions -fno-rtti -nostdlib -Wextra -Wall -Wundef -Werror)

file(GLOB_RECURSE SOURCES
        "${PROJECT_SOURCE_DIR}/*.cpp"
        "${PROJECT_SOURCE_DIR}/*.s"
)

set(LINKER_SCRIPT ${PROJECT_SOURCE_DIR}/linker.ld)
set(CMAKE_EXE_LINKER_FLAGS -ffreestanding -nostdlib -lgcc)

set(KERNEL_BIN "i686-kernel.bin")

add_custom_target(Kernel ALL
        DEPENDS ${SOURCES}
        COMMAND ${CMAKE_CXX_COMPILER} -c ${SOURCES} ${CMAKE_CXX_FLAGS}
        COMMAND ${CMAKE_CXX_COMPILER} -T ${LINKER_SCRIPT} -o ${KERNEL_BIN} ${CMAKE_EXE_LINKER_FLAGS} ${PROJECT_BINARY_DIR}/*.o
        COMMAND qemu-system-i386 -kernel ${PROJECT_BINARY_DIR}/${KERNEL_BIN}
        COMMENT "Building and running Kernel with qemu"
)

