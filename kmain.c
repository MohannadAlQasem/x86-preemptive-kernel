#include "pic.h"
#include "idt.h"
#include "GDT.h"
#include "vga.h"
#include "keyboard.h"
#include "timer.h"
#include "thread.h"

void kmain(void) {
    /* ---- Phase 1: (segments + interrupts) ---- */
    initGDT();      // segment descriptors
    initIDT();      // exception + IRQ vectors point to assembly stubs
    pic_remap();
    tasks_init();
    timer_init(100);
    keyboardInt(); /* PIC IRQs land at 0x20+ instead of clashing with CPU exceptions */

    /* ---- Phase 2: Output devices ---- */
    vga_init();                       // screen ready for use
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    vga_puts("[ OK ] GDT loaded\n");
    vga_puts("[ OK ] IDT loaded\n");
    vga_puts("[ OK ] PIC remapped\n");

    /* ---- Phase 3: Input devices ---- */

    vga_puts("[ OK ] Keyboard driver attached (IRQ1)\n\n");

    asm volatile ("sti");
    while (!task1_has_announced()) {
        asm volatile ("hlt");
    }

    vga_set_color(VGA_LIGHT_CYAN, VGA_BLACK);
    vga_puts("Type something: ");
    vga_set_color(VGA_WHITE, VGA_BLACK);

    /* ---- Phase 4: Enable interrupts and idle ---- */

    while (1) {
        keyboard_process();
        asm volatile ("hlt");   // sleep until next interrupt
    }
}
