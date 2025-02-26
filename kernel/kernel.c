

typedef unsigned long long uint64;
typedef long long int64;


const uint64 SBI_SHUTDOWN = 8;
int64 sbi_call(uint64 id, uint64 a0, uint64 a1, uint64 a2);
extern char sbss, ebss;
void clear_bss() {
    for (char *i = &sbss; i < &ebss; i++) *i = 0;
}

void shutdown() {
    sbi_call(SBI_SHUTDOWN, 0, 0, 0);
}

void printf(char *, ...);
void puts(char *);

void main() {
    // clear_bss();
    puts("Hello, world!");
    shutdown();
}