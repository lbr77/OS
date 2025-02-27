#ifndef _TRAP_H_
#define _TRAP_H_
#include "../kernel.h"
struct TrapContext {
    uint64_t x[32]; // registers
    uint64_t sstatus; // CSR Sstatus
    uint64_t sepc; // CSR sepc
};
typedef struct TrapContext TrapContext;
void trap_set_sp(TrapContext *ctx, uint64_t sp);
TrapContext * trap_app_init_context(uint64_t sp,uint64_t entry, TrapContext *ctx);
void trap_init();
TrapContext *trap_handler(TrapContext *ctx);
#endif // _TRAP_H_