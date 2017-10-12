#include <msp430.h>
/**
 * main.c
 * Blink an LED using a button press detected by an interrupt
 * MSP430FR5994
 * Author: Shani Thapa
 * Last Updated: 10/7
 * Button Interrupt
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;        // Disable the GPIO power-on default High-Z mode

    P5IE  |= BIT5;               // Interrupt Enable on Port 5;
    P5IES |= BIT5;               // Interrupt Edge Select, from High -> Low, when button is pressed
    P5IFG &= ~BIT5;              // Clear Interrupt Flag

    P5REN |= BIT5;  // enable resistor on the switch P1.1
    P5OUT |= BIT5;  // set resistor to pull up on switch P1.1
    P1DIR |= BIT0;  // set P1.0 as an output

    _BIS_SR(LPM4_bits + GIE);  // Enable global interrupt, and enable low power mode 4
}

#pragma vector=PORT5_VECTOR    // ISR
__interrupt void Port_5(void)
{
    P1OUT ^= BIT0;             // Toggle P1.0, LED1, with XOR
    P5IFG &= ~BIT5;            // Clear Interrupt flag at the end of the ISR
}
