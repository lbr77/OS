#include "kernel.h"
#include <stdarg.h> 

int64_t puts(const char *s) {
    while (*s) {
        console_putchar(*s);
        s++;
    }
    console_putchar('\n');
    return 0;
    panic("Unreachable code in puts.");
}

char base_chr[] = "0123456789ABCDEF";
int64_t put_int(int64_t x,int64_t base) {
    if(x == 0) {
        console_putchar('0');
        return 0;
    }
    if(x < 0) {
        console_putchar('-');
        x = -x;
    }
    char buf[20];
    int64_t i = 0;
    while(x) {
        buf[i++] = base_chr[x % base];
        x /= base;
    }
    for(int64_t j = i - 1; j >= 0; j--) {
        console_putchar(buf[j]);
    }
    return 0;
}

int64_t vprintf(const char *format, va_list args) {
    while(*format) {
        if(*format == '%') {
            format++;
            switch(*format) {
                case 'd': {
                    int64_t x = va_arg(args, int64_t);
                    put_int(x, 10);
                    break;
                }
                case 'x': {
                    int64_t x = va_arg(args, int64_t);
                    console_putchar('0');
                    console_putchar('x');
                    put_int(x, 16);
                    break;
                }
                case 's': {
                    const char *s = va_arg(args, const char *);
                    puts(s);
                    break;
                }
                case 'c': {
                    char c = va_arg(args, int64_t);
                    console_putchar(c);
                    break;
                }
                default:
                    console_putchar('%');
                    console_putchar(*format);
                    break;
            }
        } else {
            console_putchar(*format);
        }
        format++;
    }
    return 0;
}

int64_t printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int64_t ret = vprintf(format, args);
    va_end(args);
    return ret;
}

void panic(const char *s, ...) {
    printf("[kernel] Panic: ");
    
    va_list args;
    va_start(args, s);
    vprintf(s, args);
    va_end(args);
    
    system_shutdown();
}
