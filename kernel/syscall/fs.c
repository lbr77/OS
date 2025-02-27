
#include "../kernel.h"

int64_t sys_write(uint64_t fd, const char *buf, uint64_t len) {
    switch(fd) {
        case 1:
            for(uint64_t i = 0; i < len; i++) {
                console_putchar(buf[i]);
            }
            return len;
        default:
            panic("[kernel] sys_write: not support fd");
            return -1;
    }
}