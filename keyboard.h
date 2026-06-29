#ifndef MY_MICROKERNEL_KEYBOARD_H
#define MY_MICROKERNEL_KEYBOARD_H

#include "types.h"
#include "interrupts.h"

/*
 * Public interface of the PS/2 keyboard driver.
 *
 * The driver is split into a TOP HALF and a BOTTOM HALF
 *
 *   keyboardirq()      - TOP HALF :
 *                        Runs in hard IRQ context with interrupts
 *                        DISABLED. Must be short and constant-time. Gates the
 *                        i8042 status register, reads at most one scancode,
 *                        pushes the raw byte into the lock-free ring, returns.
 *
 *   keyboard_process() - BOTTOM HALF :
 *                         Runs in normal context from the kernel
 *                        idle loop with interrupts ENABLED. Drains the ring and
 *                        does the slow, variable-time work: scancode -> ASCII
 *                        translation, shift tracking, and VGA output.
 *
 *   keyboardInt()      - one-time init :
 *                        flush stale controller bytes, register
 *                        the IRQ1 handler, unmask IRQ1 on the PIC.
 */
void keyboardInt(void);
void keyboardirq(registers_t *r);
void keyboard_process(void);

#endif /* MY_MICROKERNEL_KEYBOARD_H */
