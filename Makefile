# Universal Makefile - Compiles C/ASM and builds an ISO

C_SOURCES = $(filter-out keyboardNEW.c, $(wildcard *.c))
ASM_SOURCES = $(wildcard *.s)
OBJ = $(C_SOURCES:.c=.o) $(ASM_SOURCES:.s=.o)

# 2. Tools & Flags
CC = gcc
CFLAGS = -m32 -g -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -c
AS = nasm
ASFLAGS = -f elf32
LDFLAGS = -T linker.ld -m elf_i386
QEMU = qemu-system-i386
GDB = gdb

# 3. Default target
all: kernel.bin

kernel.bin: $(OBJ)
	ld $(LDFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

run: kernel.bin
	$(QEMU) -kernel kernel.bin

debug: kernel.bin
	$(QEMU) -kernel kernel.bin -S -s

gdb: kernel.bin
	$(GDB) kernel.bin \
		-ex "target remote localhost:1234"

# 4. ISO Building Target (Fixed)
iso: kernel.bin
	# Create the folder structure
	mkdir -p isodir/boot/grub

	# Copy the kernel
	cp kernel.bin isodir/boot/kernel.bin

	# Generate the GRUB configuration file automatically
	echo 'menuentry "Mohannad OS" { multiboot /boot/kernel.bin }' > isodir/boot/grub/grub.cfg

	# Create the ISO image
	grub-mkrescue -o mykernel.iso isodir

clean:
	rm -f *.o kernel.bin mykernel.iso
	rm -rf isodir
