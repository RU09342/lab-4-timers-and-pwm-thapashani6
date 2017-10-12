#include <msp430.h>
/**
 * main.c
 * Use Timers to Debounce a switch to control a state of an LED
 * MSP430FR6989
 * Shani Thapa
 * 10/8/17
 * Debouncing
 */

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   //Stop Watch Dog Timer
    PM5CTL0 &= ~LOCKLPM5;       //Stop Default High-Impedance Mode
    //--------------------------LED---------------------------------------------
          P1DIR |= BIT0;                  //Set P1.0 (LED) as an output
          P1OUT &= ~BIT0;                 // LED off

          //-------------------------BUTTON-------------------------------------------
          P1DIR &= ~BIT1;                 //Button as input
          P1REN |= BIT1;                  //Enable the pull resistor

          P1IE |= BIT1;                 //Enable interrupt
          P1IES |= BIT1;                //high low
          P1IFG &= ~BIT1;               //Clear the interrupt flag register
          P1OUT |= BIT1;
          //--------------------------TIMER----------------------------------------------
          TA0CCTL0 = CCIE;                   //Enable interrupts for the first capture/compare register.
          TA0CTL = TASSEL_2 + MC_0 + ID_3;        // SMCLK, stop, /8
          TA0CCR0 = 400;

          __bis_SR_register(GIE);         //Enter interrupt mode

      }

#pragma vector=TIMER0_A0_VECTOR  // ISR for Timer0_A
__interrupt void Timer_A (void)
{
    TA0CTL = TASSEL_2 + MC_0 + ID_3;        // SMCLK, stop, /8
    P1IE |= BIT1;
}


 #pragma vector=PORT1_VECTOR         //ISR for Button 1.1
 __interrupt void Port_1(void)
 {

     P1IFG &= ~BIT1;               //P1.1 IFG cleared
     P1IE &= ~BIT1;
     TA0CTL = TASSEL_2 + MC_1 + ID_3;        // SMCLK, UP DOWN, /8
     if (!(P1IN & BIT1))
         {
             P1OUT |= BIT0;                  //Sets LED
             P1IES &= ~BIT1;                 // Clear edge detect to rising edge
         }
     else
         {
             P1OUT &= ~BIT0;               //Resets LED
             P1IES |= BIT1;           // Set edge detect to rising edge
         }
}
