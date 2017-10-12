#include <msp430.h>
/**
 * main.c
 * Use Timers to Debounce a switch to control a state of an LED
 * MSP430G2553
 * Shani Thapa
 * 10/8/17
 * Debouncing
 */

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    //--------------------------LED---------------------------------------------
    P1DIR |= BIT0;                  //Set P1.0 (LED) as an output
    P1OUT &= ~BIT0;                 // LED off

    //-------------------------BUTTON-------------------------------------------
    P1DIR &= ~BIT3;                 //Button as input
    P1REN |= BIT3;                  //Enable the pull resistor on P1.3

    P1IE |= BIT3;                 //Enable interrupt on P1.3
    P1IES |= BIT3;                //high low
    P1IFG &= ~BIT3;               //Clear the interrupt flag register on P1.3
    P1OUT |= BIT3;
    //--------------------------TIMER----------------------------------------------
    TA0CCTL0 = CCIE;                   //Enable interrupts for the first capture/compare register.
    TA0CTL = TASSEL_2 + MC_0 + ID_3;        // SMCLK, stop, /8
    TA0CCR0 = 400;



    __bis_SR_register(GIE);         //Enter interrupt mode

}

#pragma vector=TIMER0_A0_VECTOR       // ISR for Timer0_A
__interrupt void Timer_A (void)
{
    TA0CTL = TASSEL_2 + MC_0 + ID_3;        // SMCLK, stop, /8
    P1IE |= BIT3;

}

#pragma vector=PORT1_VECTOR         //ISR for Button P1.3
__interrupt void Port_1(void)
{

    P1IE &= ~BIT3;
    P1IFG &= ~BIT3;               //P1.3 IFG cleared
    TA0CTL = TASSEL_2 + MC_1 + ID_3;        // SMCLK, UP DOWN, /8
    if (!(P1IN & BIT3))
          {
            P1OUT |= BIT0;                  //Sets LED
            P1IES &= ~BIT3;                 // Clear edge detect to rising edge
          }
    else
          {
            P1OUT &= ~BIT0;               //Resets LED
            P1IES |= BIT3;           // Set edge detect to rising edge
          }
}
