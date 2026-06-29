#include "interrupts.h"
#include "idt.h"
#include "pic.h"
#include "io.h"
#include "thread.h"

static void (*irq_handlers[16])(registers_t*) = { NULL };

void register_irq_handler(uint8_t irq, void (*handler)(registers_t*)) {
    irq_handlers[irq] = handler;
}


void isr_handler(registers_t* r) {

    char msg[] = "EXCEPTION #00\n";
    msg[11] = '0' + (r->int_no / 10);
    msg[12] = '0' + (r->int_no % 10);
    for (char* p = msg; *p; p++) outb(0x3F8, *p);

    // Freeze on exception (
    while (1) asm volatile("hlt");
}


uint32_t irq_handler(registers_t* r) {
    uint8_t irq = r->int_no - 0x20;   // 0x20-0x2F → 0-15
    if (irq < 16 && irq_handlers[irq] != NULL) {
        irq_handlers[irq](r); 
    }
    pic_send_eoi(irq); //  EOI (End Of Interrupt)
    if (irq == 0) {
        return schedule((uint32_t)r); // returns next thread's esp
    }

    return (uint32_t)r;
}


