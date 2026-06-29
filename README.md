# Simple x86 Kernel with Preemptive Scheduling

A freestanding 32-bit x86 (protected mode) microkernel built from scratch in C
and NASM assembly. 

- No libc, no heap, static allocation throughout. 
- Simple early stage **preemptive, timer-driven round-robin scheduler** 
- PIT fires IRQ0 at ~100 Hz, and on each tick the kernel can switch between kernel contexts.
- The current demo switches from the boot/idle context into a fabricated task frame and keeps a PS/2 keyboard driver echoing input through the idle loop.

## What it does

- GDT + IDT setup; 8259 PIC remapped to vectors 0x20–0x2F
- PIT programmed to ~100 Hz (IRQ0) as the preemption source
- Simple Round-robin scheduling 
- PS/2 keyboard driver: top half + ring-buffered bottom half
- VGA text console

## Build & Run

Requires `nasm`, `gcc` with 32-bit support (`gcc-multilib`), and
`qemu-system-i386`. ISO creation additionally requires `grub-mkrescue` and
`xorriso`.

```sh
# Build the kernel image
make

# Run under QEMU
make run

# Run with a GDB stub for inspection
make debug
# then in another shell:
make gdb

# Optional: build a bootable ISO
make iso
qemu-system-i386 -cdrom mykernel.iso
```

Expected behavior: the kernel prints boot status lines, the scheduler switches
into `task1_entry()` and prints `Task 1 is Live`, then the prompt appears and
typed characters echo live through the keyboard bottom half.

## Architecture

```
PIT (IRQ0, ~100 Hz)
   -> irqN stub (isr.s): builds full register frame on the current task's stack
   -> irq_handler (C):   acknowledges PIC (EOI), calls schedule()
   -> schedule (C):      POLICY — saves outgoing ESP, picks next task, returns its ESP
   -> mov esp, eax:      MECHANISM — swaps the stack pointer = the context switch
   -> popa / iret:       restores the next task and resumes it
```

## Design decisions 

- **The stack pointer is the entire thread context.** On an interrupt the CPU
  and stub save the full register frame onto the running task's own stack, so a
  context switch reduces to redirecting ESP. The TCB is just a saved ESP plus a
  state flag — no separate register save area.

- **Policy vs. mechanism are strictly separated.** `schedule()` only decides
  *which* task runs and returns its saved ESP; the single `mov esp, eax` in the
  stub *makes* it run. Swapping round-robin for a different policy touches no
  assembly.

- **`eflags = 0x202` in a fabricated frame is load-bearing.** Bit 9 (IF) must be
  set so the first `iret` into a never-run task re-enables interrupts — without
  it preemption stops after the first switch.

- **Fabricated frames mirror the live frame byte-for-byte.** A never-run task has
  no real interrupt frame, so one is forged to match `registers_t` exactly; the
  generic restore path cannot tell the difference. The saved ESP points at the
  lowest slot (`ds`), where the restore begins.

- **EOI is decoupled from scheduling.** The PIC is acknowledged before the switch
  decision, so a context switch never perturbs interrupt acknowledgement.

- **Top-half / bottom-half split (keyboard).** The IRQ handler is short and
  bounded — gate the i8042 status register, read at most one byte, push to a
  lock-free ring, return. Slow work (scancode→ASCII, output) runs in a bottom
  half from the idle loop with interrupts enabled.

- **Fail-closed device handling.** Status-before-data ordering on the i8042,
  explicit overrun handling, and sanitization of unknown scancodes.



