#ifndef MY_MICROKERNEL_INTERRUPTS_H
#define MY_MICROKERNEL_INTERRUPTS_H


#include "types.h"

typedef struct registers {
    uint32_t ds;                    // data segment
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pusha
    uint32_t int_no, err_code;      // our pushed values
    uint32_t eip, cs, eflags;       // CPU pushed (same-privilege ring0->ring0)
    // Note: useresp, ss are pushed by the CPU ONLY on a privilege change (ring3->ring0).
    // Every interrupt in this kernel is ring0->ring0, so they are NOT on the stack.
} registers_t;


void register_irq_handler(uint8_t irq, void (*handler)(registers_t*));

void isr_handler(registers_t* r);
uint32_t irq_handler(registers_t* r);


extern void isr0 (void);  extern void isr1 (void);  extern void isr2 (void);
extern void isr3 (void);  extern void isr4 (void);  extern void isr5 (void);
extern void isr6 (void);  extern void isr7 (void);  extern void isr8 (void);
extern void isr9 (void);  extern void isr10(void);  extern void isr11(void);
extern void isr12(void);  extern void isr13(void);  extern void isr14(void);
extern void isr15(void);  extern void isr16(void);  extern void isr17(void);
extern void isr18(void);  extern void isr19(void);  extern void isr20(void);
extern void isr21(void);  extern void isr22(void);  extern void isr23(void);
extern void isr24(void);  extern void isr25(void);  extern void isr26(void);
extern void isr27(void);  extern void isr28(void);  extern void isr29(void);
extern void isr30(void);  extern void isr31(void);

extern void irq0 (void);  extern void irq1 (void);  extern void irq2 (void);
extern void irq3 (void);  extern void irq4 (void);  extern void irq5 (void);
extern void irq6 (void);  extern void irq7 (void);  extern void irq8 (void);
extern void irq9 (void);  extern void irq10(void);  extern void irq11(void);
extern void irq12(void);  extern void irq13(void);  extern void irq14(void);
extern void irq15(void);







#endif //MY_MICROKERNEL_INTERRUPTS_H
