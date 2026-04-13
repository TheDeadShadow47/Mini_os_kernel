CC := x86_64-elf-gcc
LD := x86_64-elf-ld

# finds all src files in src
c_source_files   := $(shell find src -name "*.c")
asm_source_files := $(shell find src -name "*.asm")

# map all src  files to build
c_object_files   := $(patsubst src/%.c, build/%.o, $(c_source_files))
asm_object_files := $(patsubst src/%.asm, build/%.o, $(asm_source_files))

all_object_files := $(c_object_files) $(asm_object_files)

# to use include 
build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) -c -I src/include -ffreestanding $< -o $@

# to assemble nasm files
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