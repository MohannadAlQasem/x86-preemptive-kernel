#include "Ascii.h"
#include "types.h"


static const char scancode_to_ascii_lower[128] = {
        0,    27,  '1', '2', '3', '4', '5', '6', '7', '8',   /* 0x00-0x09 */
        '9',  '0', '-', '=', '\b','\t','q', 'w', 'e', 'r',   /* 0x0A-0x13 */
        't',  'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,    /* 0x14-0x1D */
        'a',  's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',   /* 0x1E-0x27 */
        '\'', '`',  0,  '\\','z', 'x', 'c', 'v', 'b', 'n',   /* 0x28-0x31 */
        'm',  ',', '.', '/', 0,   '*', 0,  ' ', 0,   0,      /* 0x32-0x3B */
        0
};

static const char scancode_to_ascii_upper[128] = {
        0,    27,  '!', '@', '#', '$', '%', '^', '&', '*',
        '(',  ')', '_', '+', '\b','\t','Q', 'W', 'E', 'R',
        'T',  'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
        'A',  'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
        '"',  '~',  0,  '|', 'Z', 'X', 'C', 'V', 'B', 'N',
        'M',  '<', '>', '?', 0,   '*', 0,  ' ', 0,   0,
        0
};

char scancode_to_ascii(uint8_t scancode, int shift_pressed) {
    if(scancode >= 128)
        return 0;

    if (shift_pressed) {
        return scancode_to_ascii_upper[scancode];
    } else {
        return scancode_to_ascii_lower[scancode];
    }
}
