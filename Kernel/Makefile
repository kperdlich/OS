TOOLCHAIN := ../Toolchain/Local/i686/bin
CC := $(TOOLCHAIN)/i686-elf-g++
CFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

all:
	make -C ./Entry
	make -C ./GDT
	$(CC) $(CFLAGS) -c *.cpp
	mv *.o ../Build

