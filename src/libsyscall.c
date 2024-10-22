#include <stddef.h>
#include <stdint.h>

#include "libsyscall.h"
#include "syscall.h"
#include "task.h"
size_t sys_uart_write(const char buf[], size_t size) {
    asm volatile("mov x8, #0");
    asm volatile("svc 1");
    return size;
}

size_t sys_uart_read(char buf[], size_t size) {
    asm volatile("mov x8, #1");
    asm volatile("svc 1");
    return size;
}

void sys_exec(void (*func)()) {
    asm volatile("mov x8, #2");
    asm volatile("svc 1");
}

int sys_fork() {
    asm volatile("mov x8, #3");
    asm volatile("svc 1");
    struct task_t* task = get_current();
    return task->utask.fork_id;
}

void sys_exit(int status) {
    asm volatile("mov x8, #4");
    asm volatile("svc 1");
}

void sys_kill(int pid, int signal) {
    asm volatile("mov x8, #5");
    asm volatile("svc 1");
}
