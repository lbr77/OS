#include "../kernel.h"
#include "batch.h"

AppManager app_manager;
void batch_init() {
    app_manager_init();
    puts("[kernel] Batch inited.");
    app_mamager_print_app_info();
}
void __restore(uint64_t);
uint8_t KERNEL_STACK[KERNEL_STACK_SIZE], USER_STACK[USER_STACK_SIZE];
const uint8_t *KERNEL_TOP = KERNEL_STACK + KERNEL_STACK_SIZE;
const uint8_t *USER_TOP = USER_STACK + USER_STACK_SIZE;
void batch_run_next_app() {
    app_manager_load_app(app_manager.current_app);
    app_manager.current_app++;
    puts("[Batch] Run next app.");
    __restore((uint64_t)trap_app_init_context(
        APP_BASE_ADDRESS,
        (uint64_t)USER_TOP,
        (TrapContext *)(KERNEL_TOP - sizeof(TrapContext))
    ));
}