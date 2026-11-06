// RomiCar.h
// Basic Romi motor control
// CECS 447 Project 3

#ifndef ROMICAR_H
#define ROMICAR_H

#include <stdint.h>

// ============================================================
// MOTOR DIRECTION OPTION
//
// 0 = normal direction mapping
// 1 = invert forward/backward for the Romi
//
// If 'F' makes the car drive backward, leave this as 1.
// If 'F' already drives forward, change it to 0.
// ============================================================
#define ROMI_MOTOR_INVERTED  1


// PWM period used for both motors
#define ROMI_PERIOD      10000

// Speed levels
#define ROMI_STOP        1
#define ROMI_SPEED_35    3500
#define ROMI_SPEED_60    6000
#define ROMI_SPEED_80    8000
#define ROMI_SPEED_98    9800


// ============================================================
// RAW DIRECTION PATTERNS
//
// PE0 = Right DIR
// PE1 = Right SLP
// PE2 = Left DIR
// PE3 = Left SLP
// ============================================================
#define ROMI_RAW_FORWARD     0x0F
#define ROMI_RAW_BACKWARD    0x0A
#define ROMI_RAW_SPIN_LEFT   0x0B
#define ROMI_RAW_SPIN_RIGHT  0x0E


// ============================================================
// LOGICAL DIRECTION PATTERNS
//
// The rest of the project uses these names.
// Changing ROMI_MOTOR_INVERTED above automatically fixes
// Free Drive and Demo Mode together.
// ============================================================
#if ROMI_MOTOR_INVERTED

    #define ROMI_FORWARD     ROMI_RAW_BACKWARD
    #define ROMI_BACKWARD    ROMI_RAW_FORWARD
    #define ROMI_SPIN_LEFT   ROMI_RAW_SPIN_RIGHT
    #define ROMI_SPIN_RIGHT  ROMI_RAW_SPIN_LEFT

#else

    #define ROMI_FORWARD     ROMI_RAW_FORWARD
    #define ROMI_BACKWARD    ROMI_RAW_BACKWARD
    #define ROMI_SPIN_LEFT   ROMI_RAW_SPIN_LEFT
    #define ROMI_SPIN_RIGHT  ROMI_RAW_SPIN_RIGHT

#endif


// Basic driving
void Car_Forward(void);
void Car_Backward(void);
void Car_Left(void);
void Car_Right(void);
void Car_Stop(void);

// Speed control
void Car_SpeedUp(void);
void Car_SlowDown(void);

// Direct motor control used by Demo.c
void Motor_Set(uint8_t direction,
               uint16_t rightDuty,
               uint16_t leftDuty);

#endif
