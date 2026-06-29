
#ifndef MY_MICROKERNEL_THREAD_H
#define MY_MICROKERNEL_THREAD_H

#include "types.h"


typedef enum {
    TASK_READY,
    TASK_RUNNING
} task_state_t ;

typedef struct {
    uint32_t esp;
    task_state_t state;
}tcb_t;

uint32_t schedule(uint32_t cur_esp);
void tasks_init(void);
void fabricate_frame(tcb_t *t, void (*entry)(void), void *stack_top);
int task1_has_announced(void);

#endif //MY_MICROKERNEL_THREAD_H
