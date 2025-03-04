
#include "sys.h"

const int SYSCALL_EXIT = 93;
const int SYSCALL_WRITE = 64;
const int SYSCALL_YIELD = 124;
const int SYSCALL_GETTIME = 169;

int64_t syscall(uint64_t id, uint64_t a0, uint64_t a1, uint64_t a2) {
    int64_t ret;
    asm volatile (
        "mv x10, %1\n"
        "mv x11, %2\n"
        "mv x12, %3\n"
        "mv x17, %4\n"
        "ecall\n"
        "mv %0, x10\n"
        :"=r"(ret)
        :"r"(a0), "r"(a1), "r"(a2), "r"(id)
        :"memory", "x10", "x11", "x12", "x17"
        );
    return ret;
}

int64_t sys_write(int64_t fd, const char *buf, int64_t len) {
    return syscall(SYSCALL_WRITE, fd, (int64_t)buf, len);
}

int64_t sys_exit(int64_t exit_code) {
    return syscall(SYSCALL_EXIT, exit_code, 0, 0);
}

int64_t get_time() {
    return syscall(SYSCALL_GETTIME,0,0,0);
}
int64_t yield() {
    return syscall(SYSCALL_YIELD,0,0,0);
}