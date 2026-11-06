// Demo.c
// Demo-mode movement patterns
// CECS 447 Project 3

#include <stdint.h>
#include "RomiCar.h"
#include "Mode.h"
#include "SysTick.h"
#include "Demo.h"

// ============================================================
// TUNING VALUES
// Adjust these on the floor you demo on, with fresh batteries.
// ============================================================

// Straight lines and spin turns
#define DEMO_SPEED          ROMI_SPEED_60
#define TURN_SPEED          ROMI_SPEED_60

// Spin turn times (tune separately: short turns are not
// exactly half of long turns because of motor start-up lag)
#define TURN_90_MS          450
#define TURN_45_MS          250

// Pause between moves so momentum dies out
#define DEMO_PAUSE_MS       150

// Square
#define SQUARE_SIDE_MS  1200
#define SQUARE_TURN_MS  450

// Circle (this one already works, keep these)
#define CIRCLE_INNER_SPEED  3000
#define CIRCLE_MS           4500

// Figure 8: each loop has its own settings because the
// inner wheel is a different motor in each loop
#define FIG8_LOOP1_INNER    3000
#define FIG8_LOOP1_MS       4500
#define FIG8_LOOP2_INNER    3000
#define FIG8_LOOP2_MS       4500

// Zigzag
#define ZIG_SEGMENT_MS      1200


// ------------------------------------------------------------
// Delay while still checking SW1.
// Return 0 if Demo mode was exited.
// ------------------------------------------------------------
static uint8_t DemoDelay(uint32_t ms){

    uint32_t chunk;

    while(ms > 0){

        if(ms >= 10){
            chunk = 10;
        }
        else{
            chunk = ms;
        }

        DelayMs(chunk);
        ms -= chunk;

        CheckModeSwitch();

        if(mode != MODE_DEMO){
            Car_Stop();
            return 0;
        }
    }

    return 1;
}


// ------------------------------------------------------------
// Run the motors for a set time.
// PB6 duty first, PB7 duty second (same order as Motor_Set).
// Return 0 if Demo mode was exited.
// ------------------------------------------------------------
static uint8_t Move(uint8_t direction,
                    uint16_t pb6Duty,
                    uint16_t pb7Duty,
                    uint32_t ms){

    Motor_Set(direction, pb6Duty, pb7Duty);

    return DemoDelay(ms);
}


// ------------------------------------------------------------
// Stop and let the car settle.
// Return 0 if Demo mode was exited.
// ------------------------------------------------------------
static uint8_t Pause(void){

    Car_Stop();

    return DemoDelay(DEMO_PAUSE_MS);
}


// ------------------------------------------------------------
// Square: 4 x (straight, turn right 90)
// ------------------------------------------------------------
void Demo_Square(void){

    uint8_t side;

    for(side = 0; side < 4; side++){

        if(!Move(ROMI_FORWARD, DEMO_SPEED, DEMO_SPEED, SQUARE_SIDE_MS)) return;
        if(!Pause()) return;

        if(!Move(ROMI_SPIN_RIGHT, TURN_SPEED, TURN_SPEED, SQUARE_TURN_MS)) return;
        if(!Pause()) return;
    }

    Car_Stop();
}

// ------------------------------------------------------------
// Circle: one wheel faster than the other
// ------------------------------------------------------------
void Demo_Circle(void){

    if(!Move(ROMI_FORWARD, DEMO_SPEED, CIRCLE_INNER_SPEED, CIRCLE_MS)) return;

    Car_Stop();
}


// ------------------------------------------------------------
// Figure 8: one full loop, stop, then one full loop
// turning the other way. Both loops start and end at
// the same point, which is where the 8 crosses.
// ------------------------------------------------------------
void Demo_Figure8(void){

    // Loop 1: PB6 wheel on the outside
    if(!Move(ROMI_FORWARD, DEMO_SPEED, FIG8_LOOP1_INNER, FIG8_LOOP1_MS)) return;

    // Stop so the car doesn't drift into loop 2
    if(!Pause()) return;

    // Loop 2: PB7 wheel on the outside
    if(!Move(ROMI_FORWARD, FIG8_LOOP2_INNER, DEMO_SPEED, FIG8_LOOP2_MS)) return;

    Car_Stop();
}


// ------------------------------------------------------------
// Zigzag: 4 equal segments at +45 / -45 degrees
//
//   start --> turn right 45
//   seg 1 --> turn left 90
//   seg 2 --> turn right 90
//   seg 3 --> turn left 90
//   seg 4 --> turn right 45 (face the starting direction)
// ------------------------------------------------------------
void Demo_Zigzag(void){

    uint8_t seg;

    // Angle off 45 degrees so every segment is diagonal
    if(!Move(ROMI_SPIN_RIGHT, TURN_SPEED, TURN_SPEED, TURN_45_MS)) return;
    if(!Pause()) return;

    for(seg = 0; seg < 4; seg++){

        if(!Move(ROMI_FORWARD, DEMO_SPEED, DEMO_SPEED, ZIG_SEGMENT_MS)) return;
        if(!Pause()) return;

        if(seg == 3){
            break;          // no 90 turn after the last segment
        }

        // Alternate left and right 90 degree turns
        if((seg % 2) == 0){
            if(!Move(ROMI_SPIN_LEFT, TURN_SPEED, TURN_SPEED, TURN_90_MS)) return;
        }
        else{
            if(!Move(ROMI_SPIN_RIGHT, TURN_SPEED, TURN_SPEED, TURN_90_MS)) return;
        }

        if(!Pause()) return;
    }

    // Straighten back out to the starting direction
    if(!Move(ROMI_SPIN_RIGHT, TURN_SPEED, TURN_SPEED, TURN_45_MS)) return;

    Car_Stop();
}
