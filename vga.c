

#include "vga.h"
#include "types.h"

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  ((volatile uint16_t *)0xB8000)

static int      cursor_row = 0;
static int      cursor_col = 0;
static uint8_t  cur_attr   = 0x0F;   // white on black


static inline uint16_t pack_entry(char c, uint8_t attr) {

    return ((uint16_t)attr << 8) | (uint8_t)c;
}


static void vga_scroll(void) {

    for (int row = 1; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
            VGA_MEMORY[(row - 1) * VGA_WIDTH + col] =
                    VGA_MEMORY[row * VGA_WIDTH + col];
        }
    }

    uint16_t blank = pack_entry(' ', cur_attr);
    for (int col = 0; col < VGA_WIDTH; col++) {
        VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = blank;
    }
    cursor_row = VGA_HEIGHT - 1;
}


void vga_clear(void) {
    uint16_t blank = pack_entry(' ', cur_attr);
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_MEMORY[i] = blank;
    }
    cursor_row = 0;
    cursor_col = 0;
}

void vga_init(void) {
    cur_attr   = ((uint8_t)VGA_BLACK << 4) | (uint8_t)VGA_WHITE;
    cursor_row = 0;
    cursor_col = 0;
    vga_clear();
}

void vga_set_color(enum vga_color fg, enum vga_color bg) {
    cur_attr = ((uint8_t)bg << 4) | (uint8_t)fg;
}

void vga_putchar(char c) {
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
    } else if (c == '\r') {
        cursor_col = 0;
    } else if (c == '\b') {
        if (cursor_col > 0) {
            cursor_col--;
            VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_col] =
                    pack_entry(' ', cur_attr);
        }
    } else if (c == '\t') {
        for (int i = 0; i < 4; i++) vga_putchar(' ');
        return;
    } else {
        VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_col] =
                pack_entry(c, cur_attr);
        cursor_col++;
    }

    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;
    }
    if (cursor_row >= VGA_HEIGHT) {
        vga_scroll();
    }
}

void vga_puts(const char *s) {
    if (s == NULL) return;
    while (*s) {
        vga_putchar(*s++);
    }
}