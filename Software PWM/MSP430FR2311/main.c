#include <msp430.h> 
/**
 * main.c
 * Create a PWM using code
 * MSP430FR2311
 * Shani Thapa
 * 10/8/17
 * Software PWM
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    //needed to reset device

    P1DIR = BIT0;               //sets LED at P1.0 output PWM
    P2DIR = BIT0;               //sets LED at P2.0 output 2nd led

    PM5CTL0 &= ~LOCKLPM5;

    P1REN = BIT1; //Resistor enabled for button P1.1
    P1OUT = BIT1; //Sets resistor to pull up for button

    __bis_SR_register(GIE); //global interrupt enable (enables all maskable interrupts)

    P1IFG &= ~BIT1; //Clears interrupt flag register

    P1IE |= BIT1; //interrupt enable for button P1.1
    P1IES |= BIT1; //sets interrupt to trigger on falling edge (button press)

    P1OUT |= BIT0; //initializes led on

    TB0CCTL0 = CCIE; // CCR0 interrupt enabled
    TB0CCTL1 = CCIE; // CCR0 interrupt enabled
    TB0CCR0 = 1000; //register that timer will compare itself too
    TB0CCR1 = 500; //duty cycle, initially set to 50% duty cycle

    TB0CTL = TBSSEL_2 + ID_3 + MC_1 + TBCLR; // SMCLK, divide clock by 8, UP mode, clear TBR
}

#pragma vector=PORT1_VECTOR  // ISR for Button P1.1
__interrupt void Port_1(void){

    if (! (P1IN & BIT1)) //if button is pressed
        {
            P1IES &= ~BIT1; //sets interrupt to trigger on rising edge (button release)
            P2OUT ^= BIT0; // toggle led
        }
    else //if button released
        {
            P1IES |= BIT1; //sets interupt to trigger on falling edge (button press)
            P2OUT ^= BIT0; // toggle led
        }

    if(TB0CCR1>=1000) //checks if duty cycle is less than 100%
        {
            TB0CCR1 = 0; //adds 10% to duty cycle
        }
    else
        {
            TB0CCR1 += 100; // if duty cycle is at 100% it is reset to 0% not actually 0 b/c it does not trigger on 0
        }

    P1IFG &= ~BIT1; //Clears interrupt flag register needed otherwise will be stuck in infinite loop
}

#pragma vector=TIMER0_B0_VECTOR // ISR for Timer0_B
__interrupt void TIMER0_B0_ISR(void)
{
    P1OUT ^= BIT0; //toggles LED when interrupt triggered
    TB0CCTL0 &= ~BIT0; //clears interrupt flag
}


#pragma vector=TIMER0_B1_VECTOR  // ISR for Timer1_B
__interrupt void TIMER0_B1_ISR(void)
{
    P1OUT ^= BIT0; //toggles LED when interrupt triggered
    TB0CCTL1 &= ~BIT0; //clears interrupt flag
}
