#include "trap.h"
#include "../kernel.h"

void trap_set_sp(TrapContext *ctx, uint64_t sp) { // set sp to x_2
    ctx->x[2] = sp;
}
TrapContext *trap_app_init_context(uint64_t entry, uint64_t sp, TrapContext *ctx) {
    uint64_t sstatus;
    asm volatile("csrr %0, sstatus" : "=r"(sstatus));
    sstatus &= (~1LL << 8);
    for (int i = 0; i < 32; i++) {
        ctx->x[i] = 0;
    }
    trap_set_sp(ctx, sp);
    ctx->sepc = entry;
    ctx->sstatus = sstatus;
    return ctx;
}
void __alltraps();
void trap_init() {
    asm volatile("csrw stvec, %0" ::"r"(__alltraps));
    puts("[kernel] Trap inited.");
}
TrapContext *trap_handler(TrapContext *ctx) {
    uint64_t scause, stval, sepc;
    asm volatile("csrr %0, scause" : "=r"(scause));
    asm volatile("csrr %0, stval" : "=r"(stval));
    asm volatile("csrr %0, sepc" : "=r"(sepc));
    // printf("[kernel] scause=%x stval=%x\n", scause, stval);
    switch (scause & ~(1L << 63)) {
        case 2: // IllegalInstruction
            puts("[kernel] IllegalInstruction in application");
            batch_run_next_app();
            break;
        case 8: // UserEnvCall
            ctx->sepc += 4;
            // puts("[kernel] UserEnvCall in application.");
            printf("[kernel] syscall(%d, %x, %x, %x)\n", ctx->x[17], ctx->x[10], ctx->x[11], ctx->x[12]);
            // ctx->x[10] = syscall(ctx->x[17], ctx->x[10], ctx->x[11], ctx->x[12]);
            ctx->x[10] = syscall(ctx->x[17], ctx->x[10], ctx->x[11], ctx->x[12]);
            break;
        case 7:  // StoreFault
        case 15: // StorePageFault
            puts("[kernel] PageFault in application, kernel killed it.");
            batch_run_next_app();
            break;
        default:
            printf("[kernel] Unhandled exception! scause=%x stval=%x sepc=%x\n", scause, stval, sepc);
            printf("[kernel] registers:\n");
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 8; j++) {
                    printf("x[%d]=%x ", i * 8 + j, ctx->x[i * 8 + j]);
                }
                puts("");
            }
            batch_run_next_app();
    }
    return ctx;
}