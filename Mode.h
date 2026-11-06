// Mode.h
// SW1 mode selection and onboard LED control
// CECS 447 Project 3

#ifndef MODE_H
#define MODE_H

#include <stdint.h>

#define MODE_DEMO   1
#define MODE_DRIVE  2

extern uint8_t mode;

void ModeIO_Init(void);
void CheckModeSwitch(void);
void UpdateModeLED(void);

#endif
