extern isr_handler
extern irq_handler

%define KERNEL_DATA_SEL 0x10



isr_common:
    pusha                     ; push edi esi ebp esp ebx edx ecx eax
    mov ax, ds
    push eax                  ; save old data segment

    mov ax, KERNEL_DATA_SEL   ; kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp                  ; pass pointer to registers_t
    call isr_handler
    add esp, 4                ; clean pointer

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8                ; pop int_no + err_code
    iret

irq_common:
    pusha
    mov ax, ds
    push eax

    mov ax, KERNEL_DATA_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call irq_handler
    mov esp, eax

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8
    iret


%macro ISR_NOERR 1
global isr%1
isr%1:
    push byte 0
    push byte %1
    jmp isr_common
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
    push byte %1
    jmp isr_common
%endmacro

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR  10
ISR_ERR  11
ISR_ERR  12
ISR_ERR  13
ISR_ERR  14
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR  17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_ERR  21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31


%macro IRQ 1
global irq%1
irq%1:
    push byte 0
    push byte (32 + %1)
    jmp irq_common
%endmacro

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15
