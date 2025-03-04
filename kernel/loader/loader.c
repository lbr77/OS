#include "../kernel.h"

KernelStack KERNEL_STACK[MAX_APP_NUM];
UserStack USER_STACK[MAX_APP_NUM];
void make_kernel_stack(KernelStack *kernel_stack) {
    for (int i = 0; i < KERNEL_STACK_SIZE; i++) {
        kernel_stack->data[i] = 0;
    }
}
void make_user_stack(UserStack *user_stack) {
    for (int i = 0; i < USER_STACK_SIZE; i++) {
        user_stack->data[i] = 0;
    }
}
void stack_init() {
    for (int i = 0; i < MAX_APP_NUM; i++) {
        make_kernel_stack(&KERNEL_STACK[i]);
    }
    for (int i = 0; i < MAX_APP_NUM; i++) {
        make_user_stack(&USER_STACK[i]);
    }
    puts("[kernel] Stack inited.");
}


void load_apps() {
    puts("[kernel] Loading apps.");

    extern uint64_t _num_app;
    printf("[kernel] Found %d apps\n", _num_app);
    uint64_t *num_app_ptr = &_num_app + 1;
    // num, app1_start, app2_start ... 
    // clear i caches
    asm volatile("fence.i");
    for(uint64_t i = 0; i < _num_app; i++) {
        uint64_t base = APP_BASE_ADDRESS + i * APP_SIZE;
        uint64_t app_start = *(num_app_ptr + i);
        uint64_t app_end = *(num_app_ptr + i + 1);
        printf("[kernel] Loading app %d, size %d bytes, from %x to %x\n", i, app_end - app_start, app_start, app_end);
        for(uint64_t j = base; j < base + APP_SIZE; j ++) { // clear
            *(uint8_t *)j = 0;
        }
        for(uint64_t j = app_start; j < app_end; j++) {
            *(uint8_t *)(base + j - app_start) = *(uint8_t *)j;
        }
        puts("[kernel] Copied to memory.");
    }
    puts("[kernel] Apps loaded.");
    return;
    panic("Unreachable code in load_apps.");
}

uint64_t init_app_ctx(int id) {
    uint8_t *top = (uint8_t *)KERNEL_STACK[id].data + KERNEL_STACK_SIZE;
    TrapContext *trap_ctx_ptr = (TrapContext *)(top - sizeof(TrapContext));
    trap_app_init_context(APP_BASE_ADDRESS + id * APP_SIZE,(uint64_t)USER_STACK[id].data + USER_STACK_SIZE, trap_ctx_ptr);

    TaskContext *task_cx_ptr = (TaskContext *)((uint64_t)trap_ctx_ptr - sizeof(TaskContext));
    make_restore((uint64_t)trap_ctx_ptr, task_cx_ptr);
    printf("[kernel] App (id: %d) context inited.\n", id);
    // printf("[kernel] TaskContext: %x\n", task_cx_ptr->);
    return (uint64_t)task_cx_ptr;
}