#include <msp430.h>
/**
 * main.c
 * Blink an LED using a button press detected by an interrupt
 * MSP430FR2311
 * Author: Shani Thapa
 * Last Updated: 10/7
 * Button Interrupt
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;        // Disable the GPIO power-on default high-impedance mode

    P1IE  |= BIT1;               // Interrupt Enable on Port 1;
    P1IES |= BIT1;               // Interrupt Edge Select, from High -> Low, when button is pressed
    P1IFG &= ~BIT1;              // Clear Interrupt Flag

    P1REN |= BIT1;  // enable resistor on P1.1
    P1OUT |= BIT1;  // set resistor to pull up on switch P1.1
    P1DIR |= BIT0;  // set P1.0 as an output

    _BIS_SR(LPM4_bits + GIE);  // Enable global interrupt, and enable low power mode 4
}

#pragma vector=PORT1_VECTOR    // ISR for P1.1
__interrupt void Port_1(void)
{
    P1OUT ^= BIT0;             // Toggle P1.0, LED1, with XOR
    P1IFG &= ~BIT1;            // Clear Interrupt flag at the end of the ISR
}
