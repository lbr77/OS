#ifndef _KERNEL_H_
#define _KERNEL_H_
#include "utils/types.h"
#include "trap/trap.h"
#include "syscall/syscall.h"
#include "timer/timer.h"
#include "loader/loader.h"
#include "task/task.h"

void console_putchar(char ch);
int64_t puts(const char *s);
void panic(const char *s, ...);
void system_shutdown();
void console_putchar(char ch);
void set_timer(uint64_t time) ;
int64_t printf(const char *format, ...);
#endif // _KERNEL_H_