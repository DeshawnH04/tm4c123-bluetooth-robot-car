// SysTick.h
// Millisecond delay interface for TM4C123 at 50 MHz

#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

// Blocking delay in milliseconds using the SysTick timer.
void DelayMs(uint32_t ms);

#endif
