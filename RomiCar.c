// RomiCar.c
// Basic Romi motor control
// CECS 447 Project 3

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "PWM.h"
#include "RomiCar.h"

// PE0 = Right DIR
// PE1 = Right SLP
// PE2 = Left DIR
// PE3 = Left SLP
#define DIRECTION (*((volatile unsigned long *)0x4002403C))

#define MOTION_STOP      0
#define MOTION_FORWARD   1
#define MOTION_BACKWARD  2
#define MOTION_LEFT      3
#define MOTION_RIGHT     4

static uint16_t speed = ROMI_SPEED_60;
static uint8_t motion = MOTION_STOP;

static void ApplyMotion(void);


// ------------------------------------------------------------
// Direct motor control
// PB6 / PWM0A = Right motor
// PB7 / PWM0B = Left motor
// ------------------------------------------------------------
void Motor_Set(uint8_t direction,
               uint16_t rightDuty,
               uint16_t leftDuty){

    DIRECTION = direction;

    PWM0A_Duty(rightDuty);
    PWM0B_Duty(leftDuty);
}


// ------------------------------------------------------------
// Apply current free-driving motion
// ------------------------------------------------------------
static void ApplyMotion(void){

    switch(motion){

        case MOTION_FORWARD:

            DIRECTION = ROMI_FORWARD;

            PWM0A_Duty(speed);
            PWM0B_Duty(speed);

            break;


        case MOTION_BACKWARD:

            DIRECTION = ROMI_BACKWARD;

            PWM0A_Duty(speed);
            PWM0B_Duty(speed);

            break;


        case MOTION_LEFT:

            // Both motors move forward.
            DIRECTION = ROMI_FORWARD;

            // Physical LEFT turn:
            // right motor faster, left motor slower
            PWM0A_Duty(speed);
            PWM0B_Duty(speed / 2);

            break;


        case MOTION_RIGHT:

            // Both motors move forward.
            DIRECTION = ROMI_FORWARD;

            // Physical RIGHT turn:
            // right motor slower, left motor faster
            PWM0A_Duty(speed / 2);
            PWM0B_Duty(speed);

            break;


        case MOTION_STOP:
        default:

            PWM0A_Duty(ROMI_STOP);
            PWM0B_Duty(ROMI_STOP);

            break;
    }
}


// ------------------------------------------------------------
// Basic driving
// ------------------------------------------------------------
void Car_Forward(void){

    motion = MOTION_FORWARD;
    ApplyMotion();
}


void Car_Backward(void){

    motion = MOTION_BACKWARD;
    ApplyMotion();
}


void Car_Left(void){

    motion = MOTION_LEFT;
    ApplyMotion();
}


void Car_Right(void){

    motion = MOTION_RIGHT;
    ApplyMotion();
}


void Car_Stop(void){

    motion = MOTION_STOP;
    ApplyMotion();
}


// ------------------------------------------------------------
// Speed control
// ------------------------------------------------------------
void Car_SpeedUp(void){

    if(speed == ROMI_SPEED_35){

        speed = ROMI_SPEED_60;
    }

    else if(speed == ROMI_SPEED_60){

        speed = ROMI_SPEED_80;
    }

    else if(speed == ROMI_SPEED_80){

        speed = ROMI_SPEED_98;
    }

    ApplyMotion();
}


void Car_SlowDown(void){

    if(speed == ROMI_SPEED_98){

        speed = ROMI_SPEED_80;
    }

    else if(speed == ROMI_SPEED_80){

        speed = ROMI_SPEED_60;
    }

    else if(speed == ROMI_SPEED_60){

        speed = ROMI_SPEED_35;
    }

    ApplyMotion();
}
