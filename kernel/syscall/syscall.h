#ifndef _SYSCALL_H_
#define _SYSCALL_H_
#include "../kernel.h"
#define SYSCALL_WRITE 64
#define SYSCALL_EXIT  93
int64_t syscall(uint64_t id, uint64_t arg0, uint64_t arg1, uint64_t arg2);
int64_t sys_write(uint64_t fd, const char *buf, uint64_t len);
void sys_exit(int32_t exit_code);
#endif // _SYSCALL_H_