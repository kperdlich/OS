TOOLCHAIN := ../Toolchain/Local/i686/bin
CC := $(TOOLCHAIN)/i686-elf-g++
CFLAGS=-Wall -O -fstrength-reduce -fomit-frame-pointer -nostdinc -fno-builtin -fno-strict-aliasing -fno-common -fno-stack-protector -I$(SRC)
SRC=./Source
ASM=nasm
ASMELF=elf
ASMFLAGS=-O32 -f

all:
	make -C ./Entry
	$(CC) $(CFLAGS) -c *.c
	$(ASM) $(ASMFLAGS) $(ASMELF) gdt_setup.asm
	$(ASM) $(ASMFLAGS) $(ASMELF) interrupts.asm
	mv *.o ../Build

clean:
	rm -f *.o