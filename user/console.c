#include "sys.h"
// #include <string.h>
int strlen(const char *s) {
    int len = 0;
    while (*s) {
        len++;
        s++;
    }
    return len;
}
void puts(const char *s) {
    write(1, s, strlen(s));
}
