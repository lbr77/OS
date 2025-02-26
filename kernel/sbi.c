typedef unsigned long long uint64;
typedef long long int64;


int64 sbi_call(uint64 id, uint64 a0, uint64 a1, uint64 a2) {
    int64 ret;
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

