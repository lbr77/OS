#include"syscall.h"

int64_t syscall(uint64_t id, uint64_t arg0, uint64_t arg1, uint64_t arg2) {
    switch(id) {
        case SYSCALL_WRITE:
            return sys_write(arg0, (const char *)arg1, arg2);
        case SYSCALL_EXIT:
            // sys_exit(arg0);
            exit_and_run_next(arg0);
            return 0;
        case SYSCALL_YIELD:
            // yield();
            suspend_and_run_next();
            return 0;
        case SYSCALL_GETTIME:
            return get_time_ms();
        default:
            printf("\n[kernel] syscall: not support syscall %d\n",id);
            exit_and_run_next(-1);
            return -1;
    }
}