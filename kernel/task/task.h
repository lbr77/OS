#ifndef _TASK_H_
#define _TASK_H_
#include "../kernel.h"
typedef struct {
    uint64_t ra,s[12];
} TaskContext;
enum TaskStatus {
    UNINIT,READY,RUNNING,EXITED
};
typedef struct {
    uint64_t task_cx_ptr;
    enum TaskStatus status;
} TaskControlBlock;
extern TaskControlBlock task_list[MAX_APP_NUM];
TaskContext *init_zero_ctx(TaskContext *ctx);
TaskContext *make_restore(uint64_t ksize_ptr,TaskContext *ctx);
void task_init();
void run_first_task();
void exit_and_run_next(int64_t status);
void suspend_and_run_next();
#endif // _TASK_H_