// UART1.h
// UART1 interface for HC-06 Bluetooth data mode on TM4C123

#ifndef UART1_H
#define UART1_H

#include <stdint.h>

// Configure UART1 for 57600 baud, 8 data bits,
// 1 stop bit, odd parity.
// PB0 = U1RX, PB1 = U1TX.
void UART1_Data_Init(void);

// Return 1 when at least one received byte is available,
// otherwise return 0.
uint8_t UART1_Available(void);

// Wait for and return one received byte.
unsigned char UART1_InChar(void);

#endif
