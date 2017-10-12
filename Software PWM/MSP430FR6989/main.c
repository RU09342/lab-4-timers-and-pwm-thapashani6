#include <msp430.h> 
/**
 * main.c
 * Create a PWM using code
 * MSP430FR6989
 * Shani Thapa
 * 10/8/17
 * Software PWM
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR = BIT0; //sets LED at P1.0 output PWM
    P9DIR = BIT7; //sets LED at P9.7 output 2nd led

    P1REN = BIT1; //Resistor enabled for button P1.1
    P1OUT = BIT1; //Sets resistor to pull up for button

    __bis_SR_register(GIE); //global interrupt enable (enables all maskable interrupts)

    P1IFG &= ~BIT1; //Clears interrupt flag register
    P1IE |= BIT1; //interrupt enable for button P1.1
    P1IES |= BIT1; //sets interrupt to trigger on falling edge (button press)

    P1OUT |= BIT0; //initializes led on

    TA0CCTL0 = CCIE; // CCR0 interrupt enabled
    TA0CCTL1 = CCIE; // CCR0 interrupt enabled
    TA0CCR0 = 1000; //register that timer will compare itself too
    TA0CCR1 = 500; //duty cycle, initially set to 50% duty cycle
    TA0CTL = TASSEL_2 + ID_3 + MC_1 + TACLR; // SMCLK, divide clock by 8, Up mode, clear TAR

}

#pragma vector=PORT1_VECTOR  // ISR for Button P1.1
__interrupt void Port_1(void)
{

    if (!(P1IN&BIT1)) //if button is pressed
        {
            P1IES &= ~BIT1; //sets interrupt to trigger on rising edge (button release)
            P9OUT^= BIT7; // toggle led
        }
    else //if button released
        {
            P1IES |= BIT1; //sets interrupt to trigger on falling edge (button press)
            P9OUT^= BIT7; // toggle led
        }

    if(TA0CCR1 >= 1000) //checks if duty cycle is less than 100%
        {
            TA0CCR1 = 0; // if duty cycle is at 100% it is reset to 0%
        }
    else
        {
            TA0CCR1 += 100; //  adds 10% to duty cycle
        }

    P1IFG &= ~BIT1; //Clears interupt flag register needed otherwise will be stuck in infinite loop
}

#pragma vector=TIMER0_A0_VECTOR // ISR for Timer0_A
__interrupt void TIMER0_A0_ISR(void){
    P1OUT ^= BIT0; //toggles LED when interrupt triggered
    TA0CCTL0 &= ~BIT0; //clears interrupt flag
}

#pragma vector=TIMER0_A1_VECTOR // ISR for Timer1_A
__interrupt void TIMER0_A1_ISR(void){
    P1OUT ^= BIT0; //toggles LED when interrupt triggered
    TA0CCTL1 &= ~BIT0; //clears interrupt flag
}
