#include "../kernel.h"
void set_next_trigger(){
    uint64_t time = get_time();
    uint64_t interval = FREQ / TPS;
    set_timer(time + interval);
    printf("[kernel] Set next trigger to %d ms.\n",interval / (FREQ / MPS));
}

uint64_t get_time_ms() {
    return get_time() / (FREQ / MPS);
}

uint64_t get_time() {
    uint64_t time;
    asm volatile("rdtime %0" : "=r"(time));
    return time;
}
