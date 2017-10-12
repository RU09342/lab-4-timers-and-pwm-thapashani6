#include <msp430.h> 
/**
 * main.c
 * Use Timers to create a Hardware PWM
 * MSP430G2553
 * Shani Thapa
 * 10/8/17
 * Hardware PWM
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1SEL = BIT6; //sets P1.6 to TA0.1
    P1SEL2 &= ~BIT6; //sets P1.6 to TA0.1
    P1DIR = BIT6 | BIT0; //sets P1.6 to TA0.1 and sets P1.0 to output (2nd LED)

    P1REN = BIT3; //Resistor enabled for button P1.3
    P1OUT = BIT3; //pull up resistor for button P1.3

    __bis_SR_register(GIE); //global interrupt enable (enables all maskable interrupts)

    TA0CCTL1 = OUTMOD_7; //PWM reset/set
    TA0CCR0 = 1000 - 1; //period
    TA0CCR1 = 500; //duty cycle initially set to 50%

    P1IFG &= ~BIT3; //Clears interrupt flag register
    P1IE |= BIT3; //interrupt enable for button P1.3
    P1IES |= BIT3; //sets interrupt to trigger on falling edge (button press)


    TA0CTL = TASSEL_2 + ID_3 + MC_1 + TACLR; // SMCLK, divide clock by 8, up mode, clear TAR
}

#pragma vector=PORT1_VECTOR // ISR for Button P1.3
__interrupt void Port_1(void)
{
    if (!(P1IN&BIT3)) //if button pressed
        {
            P1IES &= ~BIT3; //sets interrupt to trigger on rising edge (button release)
            P1OUT^= BIT0; // toggle 2nd led
            }
    else //if button released
        {
            P1IES |= BIT3; //sets interrupt to trigger on falling edge (button press)
            P1OUT &=~ BIT0; // toggle led
        }

    if(TA0CCR1 >= 1000) //checks if duty cycle is less than 100%
        {
            TA0CCR1 = 1; // if duty cycle is at 100% it is reset to 0%
        }
    else
        {
            TA0CCR1 += 100; // adds 10% to duty cycle
        }

    P1IFG &= ~BIT3; //Clears interrupt flag register needed otherwise will be stuck in infinite loop
}
