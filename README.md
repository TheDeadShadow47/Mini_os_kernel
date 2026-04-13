# MiniOS-64
> A bare-metal x86_64 kernel featuring a custom shell and hardware integration.

### Quick Start
If you have **Docker** and **QEMU** ready, you can see the kernel in action in under 60 seconds:

```bash
# 1. Build the toolchain environment
docker build BuildEnv -t myos-buildenv .

# 2. Compile the ISO (inside the container)
docker run --rm -it -v "${pwd}:/root/env" myos-buildenv
make build-x86_64
exit

# 3. Launch the OS (Windows path)
& "C:\Program Files\qemu\qemu-system-x86_64.exe" -cdrom dist/x86_64/kernel.iso

# 4. Clean up
docker rmi myos-buildenv -f
````

-----

### The Tech Stack

This project demonstrates a full transition from **Real Mode** to **Long Mode**.

  * **Boot**: Multiboot-compliant header with a 64-bit switch in NASM.
  * **Memory**: Custom GDT (Global Descriptor Table) setup.
  * **Interrupts**: Full IDT implementation with PIC remapping for clean hardware IRQs.
  * **Drivers**:
      * **VGA**: Direct buffer manipulation for high-speed text rendering.
      * **Keyboard**: Scancode-to-ASCII translation layer.
      * **RTC**: Direct CMOS port communication for real-time tracking.

### The Shell

The kernel boots directly into an interactive CLI. Available commands:

  * `TIME` - Queries the CMOS for current hardware seconds.
  * `ECHO <msg>` - Tests buffer handling and string parsing.
  * `CLEAR` - Re-initializes the VGA buffer.
  * `HELP` - Lists available system commands.
  * `VERSION` - Displays kernel metadata.

-----

### Architecture

```text
src/
├── boot/      # Assembly entry points & CPU initialization
├── drivers/   # Hardware logic (Keyboard, VGA, RTC, IDT)
├── kernel/    # Shell logic & Main execution loop
└── include/   # Centralized header definitions
```

### Milestone Status

  - [x] 64-bit Long Mode Switch
  - [x] Interrupt Handling (Keyboard/Timer)
  - [x] RTC CMOS Integration
  - [x] Clean Git Flow (Main/Develop)

-----

**Maintained by:** [Aymen Hakkaoui]
**Version:** `1.0.0`

````