#include "io.h"
#include "pic.h"
#include "types.h"
#define PIC1_COMMAND 0x20
#define PIC1_DATA    (PIC1_COMMAND + 1)
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    (PIC2_COMMAND + 1)
#define PIC_EOI      0x20



void pic_remap(void) {
    uint8_t mask1 = inb(0x21);  // Save original masks
    uint8_t mask2 = inb(0xA1);

    // ICW1: Start initialization, expect ICW4
    outb(0x20, 0x11); io_wait();
    outb(0xA0, 0x11); io_wait();

    // ICW2: New vector offsets
    outb(0x21, 0x20); io_wait();  // Master IRQs → 0x20-0x27
    outb(0xA1, 0x28); io_wait();  // Slave IRQs → 0x28-0x2F

    // ICW3: Cascade (slave at IRQ 2)
    outb(0x21, 0x04); io_wait();
    outb(0xA1, 0x02); io_wait();

    // ICW4: 8086/88 mode
    outb(0x21, 0x01); io_wait();
    outb(0xA1, 0x01); io_wait();

    outb(0x21,  0xFF);  // Mask All Master IRQs.
    outb(0xA1, 0xFF);  // Mask all slave IRQs.
}
void pic_send_eoi(unsigned char irq) {
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}

void pic_unmask_irq(unsigned char irq) {
    uint16_t port = (irq < 8) ? 0x21 : 0xA1;
    uint8_t  bit  = (irq < 8) ? irq  : irq - 8;
    uint8_t  mask = inb(port);
    mask &= ~(1 << bit);   // Unmask
    outb(port, mask);
}

void pic_mask_irq(unsigned char irq) {
    uint16_t port = (irq < 8) ? 0x21 : 0xA1;
    uint8_t  bit  = (irq < 8) ? irq  : irq - 8;
    uint8_t  mask = inb(port);
    mask |= (1 << bit);    // set the bit to mask
    outb(port, mask);
}

