<div align="center">

# MiniOS-64

### A Bare-Metal x86_64 Operating System Built from Scratch

A lightweight educational operating system written in **C** and **NASM Assembly**, demonstrating the complete boot process from **Real Mode** to **64-bit Long Mode**. MiniOS-64 features a custom bootloader, interrupt handling, hardware drivers, a bitmap-based physical memory manager, and an interactive kernel shell.

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Assembly](https://img.shields.io/badge/NASM-Assembly-orange?style=for-the-badge)
![x86--64](https://img.shields.io/badge/x86--64-Architecture-red?style=for-the-badge)
![QEMU](https://img.shields.io/badge/QEMU-Emulator-009688?style=for-the-badge)
![GRUB](https://img.shields.io/badge/GRUB-Multiboot-blue?style=for-the-badge)
![Docker](https://img.shields.io/badge/Docker-Build_Environment-2496ED?style=for-the-badge&logo=docker&logoColor=white)

</div>

---

# 📖 Overview

**MiniOS-64** is a bare-metal operating system built entirely from scratch without relying on an existing operating system.

The project demonstrates many fundamental concepts of low-level systems programming, including CPU initialization, memory management, interrupt handling, hardware communication, and kernel development.

The operating system boots directly into an interactive command-line shell where users can interact with the kernel and communicate directly with hardware components such as the keyboard, CMOS Real-Time Clock, and CPU.

---

# ✨ Features

## 🚀 Boot Process

- Multiboot-compliant bootloader
- Real Mode → Protected Mode → Long Mode transition
- 64-bit kernel initialization
- Custom Global Descriptor Table (GDT)
- Paging initialization

---

## 🧠 Memory Management

- Bitmap-based Physical Memory Manager
- 4 KB page allocation
- Dynamic page allocation
- Free memory reporting

---

## ⚡ Interrupt Handling

- Complete Interrupt Descriptor Table (IDT)
- Programmable Interrupt Controller (PIC) remapping
- Keyboard interrupts
- Hardware timer support

---

## 🖥 Hardware Drivers

### VGA Driver

- Direct VGA text buffer manipulation
- High-performance text rendering
- Screen clearing
- Cursor management

### Keyboard Driver

- Keyboard interrupt handling
- Scancode → ASCII translation
- Interactive shell input

### RTC Driver

- Direct CMOS communication
- Reads hardware time
- Real-time clock support

### CPU Driver

- CPUID instruction support
- CPU Vendor detection
- Processor identification

---

# 💻 Interactive Shell

The kernel boots directly into a built-in command-line interface.

Available commands include:

| Command | Description |
|----------|-------------|
| `HELP` | Display all available commands |
| `VERSION` | Display kernel information |
| `TIME` | Read the current hardware time from CMOS |
| `CPU` | Display the processor vendor |
| `ECHO <message>` | Echo text back to the console |
| `CLEAR` | Clear the VGA screen |
| `MEM` | Allocate physical memory pages and display addresses |
| `FREE` | Display available physical memory |

---

# 🛠 Tech Stack

| Category | Technology |
|-----------|------------|
| Language | C |
| Assembly | NASM |
| Architecture | x86_64 |
| Boot Protocol | Multiboot |
| Build System | GNU Make |
| Emulator | QEMU |
| Build Environment | Docker |

---

# 🏗 Architecture

```text
                     BIOS / Bootloader
                             │
                             ▼
                  Multiboot-Compliant Header
                             │
                             ▼
                  Real Mode Initialization
                             │
                             ▼
              Protected Mode → Long Mode
                             │
                             ▼
                  Kernel Initialization
                             │
        ┌──────────────┬──────────────┬──────────────┐
        ▼              ▼              ▼
      Drivers        Memory         Shell
        │              │              │
        └──────────────┴──────────────┘
                     Hardware
```

---

# 📂 Project Structure

```text
src/
├── boot/          # Bootloader & CPU initialization
├── drivers/       # VGA, Keyboard, RTC, IDT
├── include/       # Header files
└── kernel/        # Kernel entry point & shell

dist/
└── x86_64/
    └── kernel.iso
```

---

# 🚀 Quick Start

## Build the Docker Environment

```bash
docker build BuildEnv -t myos-buildenv .
```

---

## Compile the Operating System

```bash
docker run --rm -it -v "${pwd}:/root/env" myos-buildenv
```

Inside the container:

```bash
make build-x86_64
```

Exit the container:

```bash
exit
```

---

## Launch the Operating System

Windows

```powershell
& "C:\Program Files\qemu\qemu-system-x86_64.exe" -cdrom dist/x86_64/kernel.iso
```

---

## Clean Up

```bash
docker rmi myos-buildenv -f
```

---

# ⚙ Requirements

Before building the project, install:

- Docker
- GNU Make
- NASM
- QEMU
- x86_64 GCC Cross Compiler

---

# 🧩 Core Components

### Bootloader

Responsible for transitioning the processor from Real Mode into 64-bit Long Mode before transferring execution to the kernel.

### Memory Manager

Implements a bitmap allocator that tracks physical memory using 4 KB pages.

### Interrupt Manager

Initializes the IDT and remaps the PIC to correctly handle hardware interrupts.

### Shell

Provides an interactive command-line interface for interacting directly with kernel functionality.

### Drivers

Implements low-level communication with:

- VGA
- Keyboard
- RTC
- CPU

---

# ⚡ Current Milestones

- ✅ 64-bit Long Mode
- ✅ Global Descriptor Table
- ✅ Interrupt Descriptor Table
- ✅ PIC Remapping
- ✅ Bitmap Physical Memory Manager
- ✅ Keyboard Driver
- ✅ VGA Driver
- ✅ CMOS RTC Driver
- ✅ CPUID Vendor Detection
- ✅ Interactive Kernel Shell

---

# 🔮 Future Roadmap

Planned improvements include:

- Virtual Memory Manager
- Heap Allocator
- Paging Improvements
- ATA Disk Driver
- FAT32 File System
- PCI Enumeration
- PS/2 Mouse Driver
- User Mode Support
- Multitasking Scheduler
- ELF Executable Loader
- System Calls
- Networking Stack

---

# 🤝 Contributing

Contributions are welcome!

To contribute:

```bash
git checkout -b feature/my-feature
```

```bash
git commit -m "Add awesome feature"
```

```bash
git push origin feature/my-feature
```

Then open a Pull Request.

---

# 📄 License

This project is released under the **MIT License**.

---

# 👨‍💻 Author

**Aymen Hakkaoui**

Software Engineer • Systems Programming Enthusiast

---

<div align="center">

### Built from the ground up, one instruction at a time.

⭐ If you enjoyed this project, consider giving it a star!

</div>
