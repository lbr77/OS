#ifndef _BATCH_H_
#define _BATCH_H_

#define APP_BASE_ADDRESS 0x80400000
#define USER_STACK_SIZE (4096 * 2)
#define KERNEL_STACK_SIZE (4096 * 2)

void batch_init();
void batch_run_next_app();
#define MAX_APP_NUM 10
struct AppManager {
    uint64_t num_app;
    uint64_t current_app;
    uint64_t app_start[MAX_APP_NUM + 1];
};
typedef struct AppManager AppManager;
extern AppManager app_manager;
void app_manager_init();
void app_mamager_print_app_info();
void app_manager_load_app(uint64_t app_id);
#endif // _BATCH_H_