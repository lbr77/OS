#include "task.h"
TaskControlBlock task_list[MAX_APP_NUM];
TaskContext *init_zero_ctx(TaskContext *ctx) {
    ctx->ra = 0;
    for(int i = 0; i < 12; i++) {
        ctx->s[i] = 0;
    }
    // ctx->sp = 0;
    return ctx;
}
uint64_t current_task;
extern void __restore();
TaskContext *make_restore(uint64_t ksize_ptr,TaskContext *ctx) {
    ctx->ra = (uint64_t)&__restore;
    for(int i = 0; i < 12; i++) {
        ctx->s[i] = 0;
    }
    // ctx->sp = (uint64_t)ksize_ptr;
    return ctx;
}
void task_init() {
    extern int _num_app;
    for(int i=0;i < MAX_APP_NUM; i++ ){
        task_list[i].task_cx_ptr = 0;
        task_list[i].status = UNINIT;
    }
    for(int i=0;i<_num_app; i++) {
        task_list[i].task_cx_ptr = init_app_ctx(i);
        task_list[i].status = READY;
    }
}
void __switch(uint64_t *, uint64_t *);
void run_next_task() {
    int next_task = -1;
    extern int _num_app;
    for(int i = current_task + 1; i < current_task + _num_app + 1; i++) {
        if(task_list[i % _num_app].status == READY) {
            next_task = i % _num_app;
            printf("[kernel] Next task is %d.\n", next_task);
            break;
        }
    }
    if(next_task == -1) {
        puts("[kernel] No task to run.");
        system_shutdown();
    }
    task_list[next_task].status = RUNNING;
    uint64_t *current_task_cx_ptr = (uint64_t *)&task_list[current_task].task_cx_ptr;
    uint64_t *next_task_cx_ptr = (uint64_t *)&task_list[next_task].task_cx_ptr;
    current_task = next_task;
    puts("[kernel] Switch to the next task.");
    __switch(current_task_cx_ptr, next_task_cx_ptr);
}
void exit_and_run_next(int64_t status) {
    printf("[kernel] Current task(id: %d) exited with status %d.\n",current_task, status);
    task_list[current_task].status = EXITED;
    run_next_task();
}
void suspend_and_run_next() {
    task_list[current_task].status = READY;
    run_next_task();
}
void run_first_task() {
    task_list[0].status = RUNNING;
    uint64_t t;
    puts("[kernel] Switch to the first task.");
    __switch(&t,&task_list[0].task_cx_ptr);
}