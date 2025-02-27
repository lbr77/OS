
#include "sys.h"

__attribute__((weak)) // if there is no main function, use this one.
int main() {
    puts("[user] placeholder function");
    return 0;
}
void clear_bss() {
    extern uint8_t start_bss, end_bss;
    for(uint8_t *i = &start_bss; i < &end_bss; i++) {
        *i = 0;
    }
}
__attribute__((section(".text.entry"))) // put to the start of .text section ,and it should be run first.
void _start() {
    clear_bss();
    exit(main());
}

void exit(int64_t code) {
    sys_exit(code);
}

void write(int64_t fd, const char *buf, int64_t len) {
    sys_write(fd, buf, len);
}