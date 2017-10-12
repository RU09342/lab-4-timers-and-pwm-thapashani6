#include <msp430.h> 
/**
 * main.c
 * Use Timers to create a Hardware PWM
 * MSP430FR2311
 * Shani Thapa
 * 10/8/17
 * Hardware PWM
 */

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    P2SEL0 = BIT0; // sets P2.0 to tb1.1
    P2SEL1 = ~BIT0; // sets P2.0 to tb1.1
    P2DIR = BIT0; // sets P2.0 to tb1.1
    P1DIR = BIT0; // sets P1.0 to an output for the second led

    P1REN = BIT1; //Resistor enabled for button P1.1
    P1OUT = BIT1; //pull up resistor for button P1.1

    __bis_SR_register(GIE); //global interrupt enable (enables all maskable interrupts)

    P1IFG &= ~BIT1; //Clears interrupt flag register
    P1IE |= BIT1; //interrupt enable for button P1.1
    P1IES |= BIT1; //sets interrupt to trigger on falling edge (button press)

    TB1CCTL1 = OUTMOD_7; //PWM reset/set
    TB1CCR0 = 1000; //period
    TB1CCR1 = 500; //duty cycle initially set to 50%

    TB1CTL = TBSSEL_2 + ID_3 + MC_1 + TBCLR; // SMCLK, divide clock by 8, up mode, clear TAR

}

#pragma vector=PORT1_VECTOR // ISR for Button P1.1
__interrupt void Port_1(void)
{
    if (!(P1IN&BIT1)) //if button pressed
        {
            P1IES &= ~BIT1; //sets interrupt to trigger on rising edge (button release)
            P1OUT^= BIT0; // toggle led
        }
    else //if button released
        {
            P1IES |= BIT1; //sets interrupt to trigger on falling edge (button press)
            P1OUT &= ~BIT0; // toggle led
        }

    if(TB1CCR1 <= 1000) //checks if duty cycle is less than 100%
        {
            TB1CCR1 += 100; //adds 10% to duty cycle
        }
    else
        {
            TB1CCR1 = 0; // if duty cycle is at 100% it is reset to 0%
        }

    P1IFG &= ~BIT1; //Clears interrupt flag register needed otherwise will be stuck in infinite loop
}
