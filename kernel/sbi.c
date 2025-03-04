#include "kernel.h"

enum {
    SBI_SET_TIMER = 0,
    SBI_CONSOLE_PUTCHAR = 1,
    SBI_CONSOLE_GETCHAR = 2,
    SBI_CLEAR_IPI = 3,
    SBI_SEND_IPI = 4,
    SBI_REMOTE_FENCE_I = 5,
    SBI_REMOTE_SFENCE_VMA = 6,
    SBI_REMOTE_SFENCE_VMA_ASID = 7,
    SBI_SHUTDOWN = 8,
};
int64_t sbi_call(uint64_t id, uint64_t a0,uint64_t a1,uint64_t a2) {
    int64_t ret;
    asm volatile(
        "mv x10, %1\n"
        "mv x11, %2\n"
        "mv x12, %3\n"
        "mv x17, %4\n"
        "ecall\n"
        "mv %0, a0\n"
        : "=r"(ret)
        : "r"(a0), "r"(a1), "r"(a2), "r"(id)
        : "memory", "x10", "x11", "x12", "x17"
    );
    return ret;
}

void console_putchar(char ch) {
    sbi_call(SBI_CONSOLE_PUTCHAR, ch, 0, 0);
}
void system_shutdown() {
    puts("[kernel] Goodbye!");
    sbi_call(SBI_SHUTDOWN, 0, 0, 0);
    panic("Unreachable code in system_shutdown after sbi_call.");
}

void set_timer(uint64_t time) {
    sbi_call(SBI_SET_TIMER,time,0,0);
}