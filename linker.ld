
ENTRY(loader)                    /* Entry point from loader.s */

SECTIONS
{
    /* Kernel loads at 1 MiB physical/virtual (standard, avoids BIOS data) */
    . = 0x00100000;

    /* Multiboot header - MUST be absolutely first for GRUB reliability */
    .multiboot ALIGN(4) :
    {
        KEEP(*(.multiboot))      /* Force inclusion and order */
    }

    /* Executable code - 4 KiB aligned (ready for paging/isolation) */
    .text ALIGN(0x1000) :
    {
        *(.text)
        *(.text.*)
    }

    /* Read-only data (strings, constants) */
    .rodata ALIGN(0x1000) :
    {
        *(.rodata)
        *(.rodata.*)
    }

    /* Initialized data */
    .data ALIGN(0x1000) :
    {
        *(.data)
        *(.data.*)
    }

    /* BSS - stack lives here (high addresses = safe) */
    .bss ALIGN(0x1000) :
    {
        *(COMMON)
        *(.bss)
        *(.bss.*)

        _kernel_end = .;         /* Useful for heap later */
    }
}