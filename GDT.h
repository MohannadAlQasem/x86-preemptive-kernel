#ifndef GDT_H
#define GDT_H

#include "types.h"

struct gdt_entry_struct {
    uint16_t limit;
    uint16_t lower_base;
    uint8_t  middle_base;
    uint8_t  access;
    uint8_t  flags;
    uint8_t  high_base;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));


void setGDT(uint32_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
void initGDT(void);
void load_gdt(struct gdt_ptr *GDTPtr);
void load_registers(void);


#endif