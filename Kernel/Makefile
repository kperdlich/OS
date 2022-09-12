TOOLCHAIN := ../Toolchain/Local/i686/bin
CC := $(TOOLCHAIN)/i686-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra

all:
	make -C ./Entry
	$(CC) $(CFLAGS) -c *.c
	mv *.o ../Build

