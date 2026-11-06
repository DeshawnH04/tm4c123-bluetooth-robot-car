// UART1.c
// UART1 interface for HC-06 Bluetooth data mode on TM4C123

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "UART1.h"

// ============================================================
// UART1 INITIALIZATION
//
// System clock = 50 MHz
// Baud rate    = 57600
// Data bits    = 8
// Stop bits    = 1
// Parity       = odd
// PB0          = UART1 RX
// PB1          = UART1 TX
// ============================================================
void UART1_Data_Init(void){

    // Activate UART1 and GPIO Port B
    SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART1;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;

    // Disable UART while configuring
    UART1_CTL_R &= ~UART_CTL_UARTEN;

    // 50 MHz / (16 * 57600)
    // IBRD = 54
    // FBRD = 16
    UART1_IBRD_R = 54;
    UART1_FBRD_R = 16;

    // 8-bit word length, FIFO enabled, odd parity
    // PEN = 1 enables parity
    // EPS = 0 selects odd parity
    UART1_LCRH_R =
        UART_LCRH_WLEN_8 |
        UART_LCRH_FEN |
        UART_LCRH_PEN;

    // Enable UART, TX, and RX
    UART1_CTL_R |= 0x301;

    // PB0/PB1 alternate function
    GPIO_PORTB_AFSEL_R |= 0x03;

    // Digital enable
    GPIO_PORTB_DEN_R |= 0x03;

    // PB0 = U1RX, PB1 = U1TX
    GPIO_PORTB_PCTL_R =
        (GPIO_PORTB_PCTL_R & 0xFFFFFF00) |
        0x00000011;

    // Disable analog functionality
    GPIO_PORTB_AMSEL_R &= ~0x03;
}


// ============================================================
// CHECK UART DATA AVAILABLE
// ============================================================
uint8_t UART1_Available(void){

    // RXFE = 0 means receive FIFO is not empty
    if((UART1_FR_R & UART_FR_RXFE) == 0){
        return 1;
    }

    return 0;
}


// ============================================================
// RECEIVE ONE CHARACTER
// ============================================================
unsigned char UART1_InChar(void){

    while((UART1_FR_R & UART_FR_RXFE) != 0){
    }

    return (unsigned char)(UART1_DR_R & 0xFF);
}
