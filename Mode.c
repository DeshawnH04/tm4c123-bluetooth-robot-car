// Mode.c
// SW1 mode selection and onboard LED control
// CECS 447 Project 3

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Mode.h"
#include "RomiCar.h"

#define LED GPIO_PORTF_DATA_R

#define BLUE   0x04
#define GREEN  0x08
#define DARK   0x00

uint8_t mode = MODE_DEMO;

static void Debounce(void);


// ------------------------------------------------------------
// Configure PF4 = SW1 and PF1-PF3 = RGB LED
// ------------------------------------------------------------
void ModeIO_Init(void){

    volatile unsigned long delay;

    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
    delay = SYSCTL_RCGC2_R;

    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTF_CR_R |= 0x1F;

    // PF4 = SW1 input
    GPIO_PORTF_DIR_R &= ~0x10;

    // PF1, PF2, PF3 = LED outputs
    GPIO_PORTF_DIR_R |= 0x0E;

    GPIO_PORTF_AFSEL_R &= ~0x1E;
    GPIO_PORTF_PCTL_R &= ~0x000FFFF0;
    GPIO_PORTF_AMSEL_R &= ~0x1E;
    GPIO_PORTF_DEN_R |= 0x1E;

    // SW1 is active-low
    GPIO_PORTF_PUR_R |= 0x10;

    LED = DARK;
}


// ------------------------------------------------------------
// Toggle Demo / Drive mode using SW1
// ------------------------------------------------------------
void CheckModeSwitch(void){

    if((GPIO_PORTF_DATA_R & 0x10) == 0){

        Debounce();

        if((GPIO_PORTF_DATA_R & 0x10) == 0){

            Car_Stop();

            if(mode == MODE_DEMO){
                mode = MODE_DRIVE;
            }
            else{
                mode = MODE_DEMO;
            }

            UpdateModeLED();

            // Wait for button release
            while((GPIO_PORTF_DATA_R & 0x10) == 0){
            }

            Debounce();
        }
    }
}


// ------------------------------------------------------------
// Green = Demo, Blue = Free Driving
// ------------------------------------------------------------
void UpdateModeLED(void){

    if(mode == MODE_DEMO){
        LED = GREEN;
    }
    else{
        LED = BLUE;
    }
}


// ------------------------------------------------------------
// Simple mechanical switch debounce
// ------------------------------------------------------------
static void Debounce(void){

    volatile uint32_t delay;

    for(delay = 0; delay < 150000; delay++){
    }
}
