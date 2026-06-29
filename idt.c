#include "idt.h"
#include "interrupts.h"   // ← needed for isr0, irq0, etc. externs


struct idtgate IDT[256];
struct idtptr  idt_ptr;

void initIDT() {
    idt_ptr.limit = sizeof(struct idtgate) * 256 - 1;
    idt_ptr.base  = (uint32_t)&IDT;

    //  CPU Exceptions (ISRs 0-31)
    setIDTGate(0,  (uint32_t)isr0,  0x08, 0x8E);
    setIDTGate(1,  (uint32_t)isr1,  0x08, 0x8E);
    setIDTGate(2,  (uint32_t)isr2,  0x08, 0x8E);
    setIDTGate(3,  (uint32_t)isr3,  0x08, 0x8E);
    setIDTGate(4,  (uint32_t)isr4,  0x08, 0x8E);
    setIDTGate(5,  (uint32_t)isr5,  0x08, 0x8E);
    setIDTGate(6,  (uint32_t)isr6,  0x08, 0x8E);
    setIDTGate(7,  (uint32_t)isr7,  0x08, 0x8E);
    setIDTGate(8,  (uint32_t)isr8,  0x08, 0x8E);
    setIDTGate(9,  (uint32_t)isr9,  0x08, 0x8E);
    setIDTGate(10, (uint32_t)isr10, 0x08, 0x8E);
    setIDTGate(11, (uint32_t)isr11, 0x08, 0x8E);
    setIDTGate(12, (uint32_t)isr12, 0x08, 0x8E);
    setIDTGate(13, (uint32_t)isr13, 0x08, 0x8E);
    setIDTGate(14, (uint32_t)isr14, 0x08, 0x8E);
    setIDTGate(15, (uint32_t)isr15, 0x08, 0x8E);
    setIDTGate(16, (uint32_t)isr16, 0x08, 0x8E);
    setIDTGate(17, (uint32_t)isr17, 0x08, 0x8E);
    setIDTGate(18, (uint32_t)isr18, 0x08, 0x8E);
    setIDTGate(19, (uint32_t)isr19, 0x08, 0x8E);
    setIDTGate(20, (uint32_t)isr20, 0x08, 0x8E);
    setIDTGate(21, (uint32_t)isr21, 0x08, 0x8E);
    setIDTGate(22, (uint32_t)isr22, 0x08, 0x8E);
    setIDTGate(23, (uint32_t)isr23, 0x08, 0x8E);
    setIDTGate(24, (uint32_t)isr24, 0x08, 0x8E);
    setIDTGate(25, (uint32_t)isr25, 0x08, 0x8E);
    setIDTGate(26, (uint32_t)isr26, 0x08, 0x8E);
    setIDTGate(27, (uint32_t)isr27, 0x08, 0x8E);
    setIDTGate(28, (uint32_t)isr28, 0x08, 0x8E);
    setIDTGate(29, (uint32_t)isr29, 0x08, 0x8E);
    setIDTGate(30, (uint32_t)isr30, 0x08, 0x8E);
    setIDTGate(31, (uint32_t)isr31, 0x08, 0x8E);

    // Hardware IRQs (0x20-0x2F)
    setIDTGate(32, (uint32_t)irq0,  0x08, 0x8E);
    setIDTGate(33, (uint32_t)irq1,  0x08, 0x8E);
    setIDTGate(34, (uint32_t)irq2,  0x08, 0x8E);
    setIDTGate(35, (uint32_t)irq3,  0x08, 0x8E);
    setIDTGate(36, (uint32_t)irq4,  0x08, 0x8E);
    setIDTGate(37, (uint32_t)irq5,  0x08, 0x8E);
    setIDTGate(38, (uint32_t)irq6,  0x08, 0x8E);
    setIDTGate(39, (uint32_t)irq7,  0x08, 0x8E);
    setIDTGate(40, (uint32_t)irq8,  0x08, 0x8E);
    setIDTGate(41, (uint32_t)irq9,  0x08, 0x8E);
    setIDTGate(42, (uint32_t)irq10, 0x08, 0x8E);
    setIDTGate(43, (uint32_t)irq11, 0x08, 0x8E);
    setIDTGate(44, (uint32_t)irq12, 0x08, 0x8E);
    setIDTGate(45, (uint32_t)irq13, 0x08, 0x8E);
    setIDTGate(46, (uint32_t)irq14, 0x08, 0x8E);
    setIDTGate(47, (uint32_t)irq15, 0x08, 0x8E);

    load_idt(&idt_ptr);
}


// Set a single IDT gate
void setIDTGate(uint8_t index, uint32_t base, uint16_t selector, uint8_t flags) {
    IDT[index].offset_low  = base & 0xFFFF;
    IDT[index].offset_high = (base >> 16) & 0xFFFF;
    IDT[index].selector    = selector;
    IDT[index].zero        = 0;
    IDT[index].flags       = flags;
}

// Load IDT into CPU

void load_idt(const struct idtptr* ptr) {
    asm volatile (
            "lidt (%0)"
            :
            : "r" (ptr)
            : "memory"
            );
}