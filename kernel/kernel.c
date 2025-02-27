#include "kernel.h"

extern char sbss,ebss;
void kernel_init(){
    // step.1 clear bss
    for(char *i = &sbss; i < &ebss; i++){
        *i = 0;
    }
    puts("[kernel] Hello world!");
    batch_init();
    batch_run_next_app();
    system_shutdown();
}