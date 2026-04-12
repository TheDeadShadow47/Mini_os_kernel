CC := x86_64-elf-gcc
LD := x86_64-elf-ld

# 1. Dynamically find ALL source files in the src tree
# This catches drivers, kernel, and boot files regardless of nesting
c_source_files   := $(shell find src -name "*.c")
asm_source_files := $(shell find src -name "*.asm")

# 2. Map all source files to the build/ directory
# This preserves the folder structure inside build/ to avoid filename collisions
c_object_files   := $(patsubst src/%.c, build/%.o, $(c_source_files))
asm_object_files := $(patsubst src/%.asm, build/%.o, $(asm_source_files))

all_object_files := $(c_object_files) $(asm_object_files)

# 3. Rule for compiling C files
# -I src/include allows you to use #include "header.h"
build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) -c -I src/include -ffreestanding $< -o $@

# 4. Rule for assembling NASM files (idt_.asm, port_.asm, etc.)
build/%.o: src/%.asm
	@mkdir -p $(dir $@)
	nasm -f elf64 $< -o $@

.PHONY: build-x86_64
build-x86_64: $(all_object_files)
	@mkdir -p dist/x86_64
	$(LD) -n -o dist/x86_64/kernel.bin -T targets/x86_64/linker.ld $(all_object_files)
	@cp dist/x86_64/kernel.bin targets/x86_64/iso/boot/kernel.bin
	@grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso targets/x86_64/iso
	@echo "Build successful: dist/x86_64/kernel.iso"

.PHONY: clean
clean:
	rm -rf build dist