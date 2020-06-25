#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void timer_replace_isr();
void timer_restore_isr();
void timer_start();
void timer_wait_for(uint32_t dur_ms);
void timer_wait_until(uint32_t dur_ms);

#endif
