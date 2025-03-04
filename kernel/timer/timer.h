#ifndef _TIMER_H_
#define _TIMER_H_
#include "../kernel.h"
#define FREQ 12500000
#define TPS 100
#define MPS 1000
uint64_t get_time();
uint64_t get_time_ms();
void set_next_trigger();

#endif // _TIMER_H_