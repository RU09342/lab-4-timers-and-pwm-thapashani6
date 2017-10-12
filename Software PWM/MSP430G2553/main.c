#include <msp430.h> 
/**
 * main.c
 * Create a PWM using code
 * MSP430G2553
 * Shani Thapa
 * 10/8/17
 * Software PWM
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR = BIT0 | BIT6; //sets LED at P1.0 and P1.6 to output

    P1REN = BIT3; //Resistor enabled for button P1.3
    P1OUT = BIT3; //Sets resistor to pull up for button

    __bis_SR_register(GIE); //global interrupt enable (enables all maskable interrupts)

    TA0CCTL0 = CCIE; // CCR0 interrupt enabled
    TA0CCTL1 = CCIE; // CCR0 interrupt enabled
    TA0CCR0 = 1000; //register that timer will compare itself too
    TA0CCR1 = 500; //duty cycle, initially set to 50% duty cycle

    P1IFG &= ~BIT3; //Clears interrupt flag register

    P1IE |= BIT3; //interrupt enable for button P1.3
    P1IES |= BIT3; //sets interrupt to trigger on falling edge (button press)

    P1OUT |= BIT0; //initializes led on

    TA0CTL = TASSEL_2 + ID_3 + MC_1 + TACLR; // SMCLK, divide clock by 8, UP mode, clear TAR

}


#pragma vector=PORT1_VECTOR // ISR for button P1.3
__interrupt void Port_1(void)
{

    if (!(P1IN&BIT3)) // if button is pressed
        {
            P1IES &= ~BIT3; // sets interrupt to trigger on rising edge (button release)
            P1OUT^= BIT6; // toggle led
        }
    else //if button released
        {
            P1IES |= BIT3; //sets interrupt to trigger on falling edge (button press)
            P1OUT^= BIT6; // toggle led
        }

    if(TA0CCR1 >= 1000) //checks if duty cycle is less than 100%
        {
            TA0CCR1 = 0; //if duty cycle is at 100% it is reset to 0%
        }
    else
        {
            TA0CCR1 += 100; // adds 10% to duty cycle
        }

    P1IFG &= ~BIT3; //Clears interupt flag register needed otherwise will be stuck in infinite loop
}

#pragma vector=TIMER0_A0_VECTOR // ISR for Timer0_A
__interrupt void TIMER0_A0_ISR(void)
{
    P1OUT ^= BIT0; //toggles LED when interrupt triggered
    TA0CCTL0 &= ~BIT0; //clears interrupt flag
}

#pragma vector=TIMER0_A1_VECTOR //ISR for Timer1_A
__interrupt void TIMER0_A1_ISR(void)
{
    P1OUT ^= BIT0; //toggles LED when interrupt triggered
    TA0CCTL1 &= ~BIT0; //clears interrupt flag
}
