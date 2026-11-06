// SysTick.c
// Millisecond delay using the TM4C123 SysTick timer
// Assumes the system clock has already been configured to 50 MHz.

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "SysTick.h"

// ============================================================
// MILLISECOND DELAY USING SYSTICK
//
// System clock = 50 MHz
// 50,000 cycles = approximately 1 ms
// ============================================================
void DelayMs(uint32_t ms){

    uint32_t i;

    // Disable SysTick during setup
    NVIC_ST_CTRL_R = 0;

    // 50,000 clock cycles = 1 ms at 50 MHz
    NVIC_ST_RELOAD_R = 50000 - 1;

    // Clear current value
    NVIC_ST_CURRENT_R = 0;

    // Enable SysTick using the system clock
    NVIC_ST_CTRL_R = 0x00000005;

    for(i = 0; i < ms; i++){

        // Wait for COUNT flag
        while((NVIC_ST_CTRL_R & 0x00010000) == 0){
        }
    }

    // Disable SysTick
    NVIC_ST_CTRL_R = 0;
}
