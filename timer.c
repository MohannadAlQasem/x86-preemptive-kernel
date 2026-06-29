#include "types.h"
#include "timer.h"
#include "interrupts.h"
#include "io.h"
#include "pic.h"

#define COMMAND_PORT 0x43
#define CMD_BYTE 0x36
#define CHANNEL_0_DATA_PORT 0x40

static volatile uint32_t ticks = 0;

static void time_handler(registers_t* r) {
    (void)r;
    ticks++;
}

void timer_init(uint32_t frequency) {
    if (frequency == 0) {
        return;
    }
    uint32_t divisor = 1193182 / frequency;
    if (divisor > 65535) {
        return;
    }

    if (divisor == 0) {
        return;
    }

    outb(COMMAND_PORT, CMD_BYTE );
    outb(CHANNEL_0_DATA_PORT, (uint8_t)(divisor & 0xFF));
    outb(CHANNEL_0_DATA_PORT, (uint8_t)((divisor >> 8) & 0xFF));
    register_irq_handler(0, time_handler);
    pic_unmask_irq(0);

}

