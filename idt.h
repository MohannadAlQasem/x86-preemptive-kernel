
#ifndef MY_MICROKERNEL_IDT_H
#define MY_MICROKERNEL_IDT_H

#include "types.h"

 struct idtgate {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t offset_high;
} __attribute__((packed));

struct idtptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

extern struct idtgate IDT[256];
extern struct idtptr  idt_ptr;

void initIDT();
void setIDTGate(uint8_t index, uint32_t base, uint16_t selector, uint8_t flags);
void load_idt(const struct idtptr *idt_ptr);

#endif