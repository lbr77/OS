#include"syscall.h"

int64_t syscall(uint64_t id, uint64_t arg0, uint64_t arg1, uint64_t arg2) {
    switch(id) {
        case SYSCALL_WRITE:
            return sys_write(arg0, (const char *)arg1, arg2);
        case SYSCALL_EXIT:
            sys_exit(arg0);
            return 0;
        default:
            panic("\n[kernel] syscall: not support syscall %d\n",id);
            return -1;
    }
}