#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

static inline void outb(unsigned short port, unsigned char value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline unsigned char inb(unsigned short port) {
    unsigned char result;
    asm volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static inline void io_wait() {
    outb(0x80, 0);
}

#endif //IO_H
