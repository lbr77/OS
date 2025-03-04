#include "kernel.h"

void kernel_init(){
    puts("[kernel] Hello world!");
    extern uint8_t sbss,ebss;
    // step.1 clear bss
    for(uint8_t *i = &sbss; i < &ebss; i++){
        *i = 0;
    }
    puts("[kernel] Cleared bss.");
    trap_init();
    stack_init();
    load_apps();
    task_init();
    enable_timer_interrupt();
    set_next_trigger();
    puts("[kernel] Inited.");
    puts("[kernel] Ready to run first app.");
    run_first_task();
    puts("[kernel] App run finished.");
    system_shutdown();
    panic("Unreachable code in kernel_init after system_shutdown.");
}