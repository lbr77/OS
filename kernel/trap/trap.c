#include "trap.h"
#include "../kernel.h"

void trap_set_sp(TrapContext *ctx, uint64_t sp) { // set sp to x_2
    ctx->x[2] = sp;
}
TrapContext *trap_app_init_context(uint64_t entry, uint64_t sp, TrapContext *ctx) {
    printf("[kernel] Trap context inited, entry=%x, sp=%x\n", entry, sp);
    uint64_t sstatus;
    asm volatile("csrr %0, sstatus" : "=r"(sstatus));
    sstatus &= ~(1LL << 8);
    printf("[kernel] sstatus=%x\n", sstatus);
    for (int i = 0; i < 32; i++) {
        ctx->x[i] = 0;
    }
    trap_set_sp(ctx, sp);
    ctx->sepc = entry;
    ctx->sstatus = sstatus;
    puts("[kernel] Trap context inited.");
    return ctx;
}
void __alltraps();
void trap_init() {
    asm volatile("csrw stvec, %0" ::"r"(__alltraps));
    puts("[kernel] Trap inited.");
}
void enable_timer_interrupt() {
    uint64_t sie;
    asm volatile("csrr %0, sie" : "=r"(sie));
    sie |= 1 << 5;
    asm volatile("csrw sie, %0" ::"r"(sie));
    puts("[kernel] Timer interrupt enabled.");
}
void puts_reg(TrapContext *ctx, uint64_t scause, uint64_t stval, uint64_t sepc) {
    printf("scause=%x stval=%x sepc=%x\n", scause, stval, sepc);
    printf("[kernel] registers:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            printf("x[%d]=%x ", i * 8 + j, ctx->x[i * 8 + j]);
        }
        puts("");
    }
}
TrapContext *trap_handler(TrapContext *ctx) {
    uint64_t scause, stval, sepc;
    asm volatile("csrr %0, scause" : "=r"(scause));
    asm volatile("csrr %0, stval" : "=r"(stval));
    asm volatile("csrr %0, sepc" : "=r"(sepc));
    // printf("[kernel] scause=%x stval=%x\n", scause, stval);
    switch (scause) {
        case 1:
            printf("[kernel] InstructionFault in application. ");
            puts_reg(ctx, scause, stval, sepc);
            // system_shutdown();
            exit_and_run_next(-1);
            break;
        case 2: // IllegalInstruction
            puts("[kernel] IllegalInstruction in application. ");
            // system_shutdown();
            exit_and_run_next(-1);
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
            // system_shutdown();
            exit_and_run_next(-1);
            break;
        case (1ULL << 63) | 5: // timer interupt
            puts("[kernel] Timer Interrupt.");
            set_next_trigger();
            suspend_and_run_next();
            break;
        default:
            printf("[kernel] Unhandled exception! ");
            puts_reg(ctx, scause, stval, sepc);
            // system_shutdown();
            exit_and_run_next(-1);
    }
    return ctx;
    panic("Unreachable code in trap_handler");
}