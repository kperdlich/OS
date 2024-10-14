TOOLCHAIN := ../Toolchain/Local/i686/bin
LD := $(TOOLCHAIN)/i686-elf-g++

OBJDIR := Build

OBJ=$(OBJDIR)/gdt_setup.o \
	$(OBJDIR)/interrupts.o \
	$(OBJDIR)/kmain.o \
	$(OBJDIR)/video.o \
	$(OBJDIR)/io.o \
	$(OBJDIR)/string.o \
	$(OBJDIR)/gdt.o \
	$(OBJDIR)/idt.o \
	$(OBJDIR)/isr.o \
	$(OBJDIR)/irq.o \
	$(OBJDIR)/timer.o \
	$(OBJDIR)/keyboard.o


all: clean
	make -C ./src
	$(LD) -T linker.ld -o OS.bin -ffreestanding -O2 -nostdlib Build/Boot.o $(OBJ) -lgcc
	qemu-system-i386 -kernel OS.bin

clean:
	rm -f OS.bin
	rm -f -d -r Build
	mkdir Build
