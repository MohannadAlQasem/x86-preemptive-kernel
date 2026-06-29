#include "keyboard.h"
#include "io.h"
#include "interrupts.h"
#include "types.h"
#include "Ascii.h"
#include "vga.h"
#include "pic.h"

// i8042 controller ports
#define KeyboardDataPort     0x60
#define KeyboardStatusPort   0x64

// i8042 status register bits (read from port 0x64)
#define KeyboardOBF    0x01   // Output Buffer Full: a byte is waiting at 0x60
#define AuxBuffer      0x20   // byte originated from the AUX/mouse channel
#define StatusTimeOut  0x40   // controller timeout error
#define StatusParity   0x80   // parity error on the received byte


#define KeyboardBufferSize  256
#define KeyboardBufferMask  (KeyboardBufferSize - 1)


typedef char kbd_buffer_size_must_be_power_of_two
    [((KeyboardBufferSize & (KeyboardBufferSize - 1)) == 0) ? 1 : -1];


#define LSHIFT_DOWN  0x2A
#define RSHIFT_DOWN  0x36
#define LSHIFT_UP    0xAA
#define RSHIFT_UP    0xB6


#define barrier() asm volatile("" ::: "memory")


static int shift_pressed = 0;


static volatile uint8_t  keyboard_buffer[KeyboardBufferSize];
static volatile uint32_t kbd_head = 0;
static volatile uint32_t kbd_tail = 0;


static volatile uint32_t kbd_dropped = 0;

static void keyboard_drain_output(void) {
    for (int i = 0; i < 32; i++) {
        if ((inb(KeyboardStatusPort) & KeyboardOBF) == 0) {
            break;
        }
        (void)inb(KeyboardDataPort);
    }
}



void keyboardirq(registers_t *r) {
    (void)r;   // the saved register frame is unused in the top half

    uint8_t status = inb(KeyboardStatusPort);

    // if Output Buffer is Empty
    if ((status & KeyboardOBF) == 0) {
        return;
    }

    // AUX (mouse) byte: not keyboard data. drop it.
    if (status & AuxBuffer) {
        (void)inb(KeyboardDataPort);
        return;
    }

    // Controller timeout. drop it.
    if (status & StatusTimeOut) {
        (void)inb(KeyboardDataPort);
        return;
    }

    // Parity error.
    if (status & StatusParity) {
        (void)inb(KeyboardDataPort);
        return;
    }

    // Reading From Data Port
    uint8_t scancode = inb(KeyboardDataPort);


    uint32_t next_head = (kbd_head + 1) & KeyboardBufferMask;

    if (next_head == kbd_tail) {
        kbd_dropped++;
        return;
    }

    keyboard_buffer[kbd_head] = scancode;
    barrier();
    kbd_head = next_head;

}


void keyboard_process(void) {

    while (kbd_tail != kbd_head) {
        uint8_t scancode = keyboard_buffer[kbd_tail];
        barrier();
        kbd_tail = (kbd_tail + 1) & KeyboardBufferMask;


        if (scancode == LSHIFT_DOWN || scancode == RSHIFT_DOWN) {
            shift_pressed = 1;
            continue;
        }
        if (scancode == LSHIFT_UP || scancode == RSHIFT_UP) {
            shift_pressed = 0;
            continue;
        }


        if (scancode & 0x80) {
            continue;
        }

        char c = scancode_to_ascii(scancode, shift_pressed);
        if (c) {
            vga_putchar(c);
        }
    }
}


void keyboardInt(void) {
    keyboard_drain_output();

    register_irq_handler(1, keyboardirq);
    pic_unmask_irq(1);
}
