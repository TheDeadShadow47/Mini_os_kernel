# MiniOS-64

A learning-focused bare-metal x86_64 kernel that boots through GRUB (Multiboot2), switches to 64-bit long mode, initializes basic hardware support, and exposes a simple command shell.

## What this project is

MiniOS-64 is a small custom operating-system kernel written in C and NASM assembly.  
It is intentionally minimal, with direct hardware access and no external runtime/standard library.

Core goals shown by this codebase:

- Multiboot2 boot path
- 32-bit startup to 64-bit long-mode transition
- Interrupt setup (IDT + PIC remap)
- PS/2 keyboard input handling
- VGA text-mode output
- RTC CMOS time read
- CPUID vendor query
- Bitmap-based physical page allocator (PMM)
- Interactive in-kernel command shell

---

## Repository layout

```text
.
├── BuildEnv/                 # Container/toolchain support files
│   ├── dockerfile            # Build image with cross-toolchain + GRUB/NASM tools
│   ├── gcc/                  # GCC patch/multilib config for x86_64-elf
│   └── src/                  # Optional scripts to build binutils/GCC
├── src/
│   ├── boot/                 # Assembly startup path and mode switch
│   ├── drivers/              # Hardware-facing kernel drivers
│   ├── include/              # Public headers/shared constants
│   └── kernel/               # Kernel entry logic + command shell
├── targets/x86_64/
│   ├── linker.ld             # Link script (kernel loaded at 1 MiB)
│   └── iso/boot/grub/grub.cfg# GRUB boot menu entry
├── Makefile                  # Build rules for objects, kernel binary, and ISO
└── LICENSE                   # MIT license
```

---

## Boot and execution flow

### 1) GRUB + Multiboot2 handoff

- `grub.cfg` loads `boot/kernel.bin` using `multiboot2`.
- `src/boot/header.asm` defines the required Multiboot2 header/magic/checksum.

### 2) 32-bit startup checks (`src/boot/main.asm`)

Startup symbol `start` performs:

1. Stack setup
2. Multiboot magic validation (`check_multiboot`)
3. CPUID availability check (`check_cpuid`)
4. Long-mode capability check (`check_long_mode`)
5. Temporary paging structures creation (`setup_page_tables`)
6. Paging + long-mode enable (`enable_paging`)
7. GDT load + far jump into 64-bit code segment

If a critical check fails, an error marker is written to VGA memory and CPU halts.

### 3) 64-bit entry (`src/boot/main64.asm`)

- `long_mode_start` zeros segment registers and calls `kernel_main`.
- `kernel_main` (in C) takes over from here.

---

## Kernel initialization (`src/kernel/main.c`)

`kernel_main` performs early runtime setup:

- Clears VGA text buffer and sets text color
- Prints welcome/banner text and prompt
- Initializes PMM with a fixed memory size (`128 MiB`) and bitmap base address (`0x1000000`)
- Marks low memory pages as used (first 256 pages)
- Initializes keyboard path (`keyboard_init`)
- Registers input callback (`keyboard_set_handler(handle_input)`)
- Enters idle loop with `hlt`

Keyboard input is collected in `input_buffer` and parsed on Enter to execute shell commands.

---

## Driver and subsystem details

### VGA text output (`src/drivers/print.c`)

Directly writes characters/attributes to `0xB8000` in 80x25 mode.

Features:

- Screen clear and row clear
- Newline handling with scroll-up behavior
- Backspace erase behavior
- Foreground/background color control
- Decimal/hex/binary `uint64_t` printing helpers

### Port I/O (`src/drivers/port.c` + `src/drivers/port_.asm`)

Assembly-backed primitives:

- `port_inb(port)`
- `port_outb(port, data)`
- `port_wait()` (read from `0x80`)

These are used by PIC/PS2/RTC code.

### PIC remap (`src/drivers/pic.c`)

Reprograms legacy PIC controllers:

- Master offset: `0x20`
- Slave offset: `0x28`
- Unmasks keyboard IRQ (IRQ1)
- Sends EOI when needed

### IDT setup (`src/drivers/idt.c` + `src/drivers/idt_.asm`)

- Creates 256-entry IDT
- Installs keyboard interrupt gate at vector `0x21`
- Loads IDT via `lidt`
- Enables interrupts via `sti`

`idt_.asm` wrapper saves/restores GP registers around C interrupt handlers and returns with `iretq`.

### PS/2 keyboard (`src/drivers/keyboard.c`, `src/drivers/ps2.c`)

- Reads scancodes from port `0x60`
- Handles optional extended prefix (`0xE0`)
- Classifies make/break events
- Translates selected scancodes to uppercase ASCII (`A-Z`, digits, space, Enter, Backspace)
- Dispatches events to a user-registered handler

### RTC (`src/drivers/rtc.c`)

Reads CMOS RTC seconds:

- Waits for update-in-progress bit to clear
- Reads stable value twice
- Converts from BCD when required

### CPU vendor query (`src/drivers/cpu.c`)

Uses `cpuid` leaf `0` to build 12-byte vendor string.

### Physical Memory Manager (`src/drivers/pmm.c`)

Simple bitmap allocator over 4 KiB pages:

- Tracks used/free pages with bit operations
- Allocates first free page by scanning bitmap
- Supports mark used/free and explicit free
- Reports free memory in bytes (`pmm_get_free_count`)

---

## Built-in shell commands

Handled in `execute_command`:

- `HELP` — list available commands
- `CLEAR` — clear screen
- `VERSION` — print kernel version string
- `TIME` — print RTC seconds
- `ECHO <text>` — echo message
- `CPU` — print CPU vendor
- `MEM` — allocate two pages and print addresses (frees one page in current implementation)
- `FREE` — print free RAM in KB

Unknown non-empty commands print a fallback message.

---

## Build system

`Makefile` automatically discovers all `*.c` and `*.asm` files under `src/` and builds objects under `build/`.

Build target:

- `make build-x86_64`
  - Compiles C with `x86_64-elf-gcc` and `-ffreestanding`
  - Assembles NASM as ELF64 objects
  - Links kernel with `x86_64-elf-ld` and `targets/x86_64/linker.ld`
  - Copies kernel to ISO tree
  - Generates bootable ISO via `grub-mkrescue`

Clean target:

- `make clean` removes `build/` and `dist/`

---

## Toolchain and environment

The project expects an `x86_64-elf` cross compiler and GRUB ISO tooling.

`BuildEnv/dockerfile` provides a containerized build environment with:

- `x86_64-elf` toolchain base image
- `nasm`
- `xorriso`
- `grub-pc-bin`
- `grub-common`

The `BuildEnv/src` scripts and GCC patch files are included for custom cross-toolchain builds/customization.

---

## Quick start

### Option A: Build in Docker

```bash
docker build -t myos-buildenv BuildEnv
docker run --rm -it -v "${PWD}:/root/env" myos-buildenv
make build-x86_64
exit
```

### Option B: Build locally (with toolchain installed)

```bash
make build-x86_64
```

The bootable ISO is generated at:

```text
dist/x86_64/kernel.iso
```

### Run in QEMU

```bash
qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso
```

---

## Current limitations and notes

- PMM initialization currently uses fixed values in `kernel_main` instead of Multiboot memory map parsing.
- Keyboard translation is limited and currently uppercase-centric.
- Command parser is simple and case-sensitive.
- Interrupt coverage is minimal (focused on keyboard).
- This is an educational kernel and not a production OS.

---

## License

MIT License. See `/home/runner/work/Mini_os_kernel/Mini_os_kernel/LICENSE`.
