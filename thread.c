#include "thread.h"
#include "vga.h"
#define N 2
static tcb_t tasks[N];
static uint8_t task_stack[N][4096] __attribute__((aligned(16)));
static int current;
static volatile int task1_announced;


void fabricate_frame(tcb_t *t, void (*entry)(void), void *stack_top) {
        uint32_t *p = (uint32_t *)stack_top;
        *(--p) = 0x202; // eflags
        *(--p) = 0x08; // cs - Kernel Code Segment
        *(--p) = (uint32_t)entry; // eip
        *(--p) = 0; // err_code
        *(--p) = 0; // int_no
        *(--p) = 0; // eax
        *(--p) = 0; // ecx
        *(--p) = 0; // edx
        *(--p) = 0; // ebx
        *(--p) = 0; // esp - ignored by 'popa'
        *(--p) = 0; // ebp
        *(--p) = 0; // esi
        *(--p) = 0; // edi
        *(--p) = 0x10; // ds - Kernel Data Segment

        // p now points as the ds slot.

        t->esp   = (uint32_t)p;
        t->state = TASK_READY;
}

// decides which task runs next.
// the actual switch is the `mov esp, eax` in the IRQ stub acting on the ESP we return.
// the task whose ESP we return is always marked TASK_RUNNING.

void task1_entry() {
        vga_set_color(VGA_WHITE, VGA_BLACK);
        vga_puts("Task 1 is Live \n\n");
        task1_announced = 1;
        for (;;) {
        }

}

int task1_has_announced(void) {
        return task1_announced;
}

void tasks_init(void) {
        fabricate_frame(&tasks[1], task1_entry, &task_stack[1][4096]);
}


uint32_t schedule(uint32_t cur_esp) {
        tasks[current].esp = cur_esp; // save the outgoing handle
        tasks[current].state = TASK_READY;
        for (int i = 0; i < N; i++) { // pick next runnable — ( round-robin )
                current = (current + 1) % N;
                if (tasks[current].state == TASK_READY) {
                        tasks[current].state = TASK_RUNNING;
                        return tasks[current].esp;  // hand back the new handle
                }

        }
        tasks[current].state = TASK_RUNNING;
        return cur_esp;
}
