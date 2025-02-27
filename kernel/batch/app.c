#include "../kernel.h"
#include "batch.h"
extern AppManager app_manager;

void app_manager_init() {
    extern uint64_t _num_app;
    uint64_t *ptr = &_num_app;
    app_manager.num_app = _num_app;
    for(int i = 0; i < app_manager.num_app + 1; i++) {
        app_manager.app_start[i] = ptr[i+1];
    }
    app_manager.current_app = 0;
    trap_init();
}
void app_mamager_print_app_info() {
    printf("[manager] num_app=%d current_app=%d\n", app_manager.num_app, app_manager.current_app);
    for(int i = 0; i < app_manager.num_app; i++) {
        printf("[manager] app_start[%d]=%x\n", i, app_manager.app_start[i]);
    }
}

void app_manager_load_app(uint64_t app_id) {
    if(app_id >= app_manager.num_app) {
        panic("\n[manager] app_id out of range.\n");
    }
    asm volatile("fence.i");
    app_manager.current_app = app_id;
    uint64_t entry = app_manager.app_start[app_id];
    for(uint64_t i=entry;i<app_manager.app_start[app_id+1];i++) {
        *(uint8_t *)(APP_BASE_ADDRESS + i - entry) = *(uint8_t *)i;
    }
}