#include "GDT.h"
#include "types.h"


static struct gdt_entry_struct gdt_entries[5];
static struct gdt_ptr          GDTPtr;

void setGDT(uint32_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    gdt_entries[index].lower_base  = (base & 0xFFFF);
    gdt_entries[index].middle_base = (base >> 16) & 0xFF;
    gdt_entries[index].high_base   = (base >> 24) & 0xFF;

    gdt_entries[index].limit = (limit & 0xFFFF);
    gdt_entries[index].flags = (limit >> 16) & 0x0F;
    gdt_entries[index].flags |= (granularity & 0xF0);

    gdt_entries[index].access = access;
}

void initGDT(void)
{
    // 0 : NULL descriptor 
    setGDT(0, 0, 0, 0, 0);

    // 1: Kernel Code
    setGDT(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // 2 : Kernel Data
    setGDT(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // 3 : User Code
    setGDT(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

    // 4: User Data
    setGDT(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    GDTPtr.limit = sizeof(gdt_entries) - 1;
    GDTPtr.base  = (uint32_t)&gdt_entries;

    load_gdt(&GDTPtr);
    load_registers();       // reloads DS/ES/FS/GS/SS + far jump
}