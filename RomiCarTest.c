// RomiCarTest.c
// CECS 447 Project 3
// Bluetooth Controlled Robot Car

#include <stdint.h>
#include "PWM.h"
#include "UART1.h"
#include "RomiCar.h"
#include "Mode.h"
#include "Demo.h"

static void HandleDriveCommand(unsigned char command);
static void HandleDemoCommand(unsigned char command);


int main(void){

    unsigned char command;

    // 50 MHz system clock
    PLL_Init();

    // Hardware PWM:
    // PB6 = right motor
    // PB7 = left motor
    PWM0A_Init(ROMI_PERIOD);
    PWM0B_Init(ROMI_PERIOD);

    // PE0-PE3 = motor direction / sleep
    PortE_Init();

    // Bluetooth UART1
    UART1_Data_Init();

    // SW1 + onboard RGB LED
    ModeIO_Init();

    // Power-up requirement:
    // Demo Mode, green LED, car stopped
    Car_Stop();
    mode = MODE_DEMO;
    UpdateModeLED();


    while(1){

        CheckModeSwitch();

        if(UART1_Available()){

            command = UART1_InChar();

            if(mode == MODE_DRIVE){
                HandleDriveCommand(command);
            }
            else{
                HandleDemoCommand(command);
            }
        }
    }
}


// ------------------------------------------------------------
// Mode 2: Free Driving
// ------------------------------------------------------------
static void HandleDriveCommand(unsigned char command){

    switch(command){

        case 'F':
        case 'f':
            Car_Forward();
            break;

        case 'B':
        case 'b':
            Car_Backward();
            break;

        case 'L':
        case 'l':
            Car_Left();
            break;

        case 'R':
        case 'r':
            Car_Right();
            break;

        case 'S':
        case 's':
            Car_Stop();
            break;

        case 'U':
        case 'u':
            Car_SpeedUp();
            break;

        case 'D':
        case 'd':
            Car_SlowDown();
            break;

        default:
            break;
    }
}


// ------------------------------------------------------------
// Mode 1: Demo
// ------------------------------------------------------------
static void HandleDemoCommand(unsigned char command){

    switch(command){

        case '8':
            Demo_Figure8();
            break;

        case 'C':
        case 'c':
            Demo_Circle();
            break;

        case 'S':
        case 's':
            Demo_Square();
            break;

        case 'Z':
        case 'z':
            Demo_Zigzag();
            break;

        default:
            break;
    }
}
