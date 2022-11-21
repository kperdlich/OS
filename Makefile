TOOLCHAIN := Toolchain/Local/i686/bin
LD := $(TOOLCHAIN)/i686-elf-g++

BUILD_DIR := Build

OBJ:=$(BUILD_DIR)/kernel.o \
	$(BUILD_DIR)/gdt.o \
	$(BUILD_DIR)/IO.o \


all: clean
	make -C ./Kernel
	$(LD) -T linker.ld -o OS.bin -ffreestanding -O2 -nostdlib Build/Boot.o $(OBJ) -lgcc
	qemu-system-i386 -kernel OS.bin

clean:
	rm -f OS.bin
	rm -f -d -r Build
	mkdir Build
