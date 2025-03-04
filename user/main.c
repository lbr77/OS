// 00 hello_world
// #include "sys.h"
// int main() {
//     puts("Hello, world!\n");
//     return 0;
// }
// 01 store_fault
// int main() {
//     *(volatile unsigned char *)0 = 0;
// }

// 02 power
// #define SIZE 10
// #define P 3
// #define STEP 100000
// #define MOD 10007

// int main() {
//     uint32_t pow[SIZE] = {0};
//     int64_t index = 0;
//     pow[index] = 1;
    
//     for (int64_t i = 1; i <= STEP; i++) {
//         uint32_t last = pow[index];
//         index = (index + 1) % SIZE;
//         pow[index] = (last * P) % MOD;
        
//         if (i % 10000 == 0) {
//             printf("%u^%zu=%u(MOD %u)\n", 
//                               P, i, pow[index], MOD);
//         }
//     }
    
//     puts("Test power OK!\n");
//     return 0;
// }

// int main() {
//     asm volatile("sret");
//     return 0;
// }

// int main() {
//     asm volatile("csrw sstatus zero");
//     return 0;
// }