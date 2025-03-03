#include "../kernel.h"

void sys_exit(int32_t exit_code) {
    printf("[kernel] Application exited with code %d.\n", exit_code); 
    system_shutdown();
}