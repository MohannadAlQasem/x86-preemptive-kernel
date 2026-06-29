global load_gdt
global load_registers

; load_gdt (struct gdt_ptr GDTPtr);
; Loads the Global Descriptor Table using the LGDT instruction.
load_gdt:
    mov eax, [esp + 4]  ; Get the pointer to the GDTR
    lgdt [eax]          ; Load GDTR From Memory
    ret
; load_registers()
; Reloads the segment registers
load_registers:
    ; Reload Data Segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush     ; Far jump to Code Segment

.flush:
    ret