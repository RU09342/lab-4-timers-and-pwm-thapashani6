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

    P1SEL0 = BIT0; //sets P1.0 to TA0.1
    P1SEL1 = ~BIT0; //sets P1.0 to TA0.1
    P1DIR = BIT0 | BIT1; //sets P1.0 to TA0.1 and P1.1 output (2nd LED)

    PM5CTL0 &= ~LOCKLPM5;

    P5REN = BIT6; //Resistor enabled for button P5.6
    P5OUT = BIT6; //pull up resistor for button P5.6

    __bis_SR_register(GIE); //global interupt enable (enables all maskable interupts)

    TA0CCTL1 = OUTMOD_7; //PWM reset/set
    TA0CCR0 = 1250; //period
    TA0CCR1 = 625; //duty cycle initially set to 50%

    P5IFG &= ~BIT6; //Clears interupt flag register
    P5IE |= BIT6; //interupt enable for button P5.6
    P5IES |= BIT6; //sets interupt to trigger on falling edge (button press)


    TA0CTL = TASSEL_2 + ID_3 + MC_1 + TACLR; // SMCLK, divide clock by 8, up mode, clear TAR

}

#pragma vector=PORT5_VECTOR //ISR for Button P5.6
__interrupt void Port_5(void)
{
    if (!(P5IN&BIT6)) //if button is pressed
        {
            P5IES &= ~BIT6; //sets interupt to trigger on rising edge (button release)
            P1OUT^= BIT1; // toggle 2nd led
        }
    else //if button released
        {
            P5IES |= BIT6; //sets interupt to trigger on falling edge (button press)
            P1OUT &= ~BIT1; // toggle led
        }

    if(TA0CCR1 <= 1000>) //checks if duty cycle is less than 100%
        {
            TA0CCR1 += 100; //adds 10% to duty cycle
        }
    else
        {
            TA0CCR1 = 0; // if duty cycle is at 100% it is reset to 0%
        }

    P5IFG &= ~BIT6; //Clears interupt flag register needed otherwise will be stuck in infinite loop
}
