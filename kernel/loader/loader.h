#ifndef _LOADER_H_
#define _LOADER_H_
#include "../kernel.h"
#define KERNEL_STACK_SIZE 4096
#define USER_STACK_SIZE 4096
#define MAX_APP_NUM 6
#define APP_BASE_ADDRESS 0x80400000
#define APP_SIZE 0x20000
typedef struct __attribute__((aligned(4096))) {
    uint64_t data[KERNEL_STACK_SIZE];
} KernelStack;
typedef struct __attribute__((aligned(4096))) {
    uint64_t data[USER_STACK_SIZE];
} UserStack;

extern KernelStack KERNEL_STACK[MAX_APP_NUM];
extern UserStack USER_STACK[MAX_APP_NUM];
void load_apps();
void stack_init();
uint64_t init_app_ctx(int id);
#endif // _LOADER_H_