#include <msp430.h> 
/**
 * main.c
 * Use Timers to create a Hardware PWM
 * MSP430F5529
 * Shani Thapa
 * 10/8/17
 * Hardware PWM
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1SEL |= BIT2; //sets P1.0 to GPIO
    P4SEL &= ~BIT7; //sets P4 to GPIO

    P1DIR |= BIT0; //sets LED at P1.0 to output
    P4DIR |= BIT7; //sets P4 to an output for the second led

    P1REN = BIT1; //Resistor enabled for button P1.1
    P1OUT |= BIT1; //Sets resistor to pull up for button

    __bis_SR_register(GIE); //global interrupt enable (enables all maskable interrupts)

    TA0CCTL0 = OUTMOD_7; //PWM reset/set
    TA0CCR0 = 1000; //period
    TA0CCR1 = 500; //duty cycle initially set to 50%

    P1IFG &= ~BIT1; //Clears interrupt flag register
    P1IE |= BIT1; //interrupt enable for button P1.1
    P1IES |= BIT1; //sets interrupt to trigger on falling edge (button press)

    TA0CTL = TASSEL_2 + ID_3 + MC_1 + TACLR; // SMCLK, divide clock by 8,  UP mode, clear TAR

}

#pragma vector=PORT1_VECTOR // ISR for Button P1.1
__interrupt void Port_1(void)
{

    //blinks 2nd led when button is pressed
    if ((P1IN&BIT1)==0x0000) //if button pressed
        {
            P1IES &= ~BIT1; //sets interrupt to trigger on rising edge (button release)
            P4OUT^= 0x80; // toggle led
        }
    else //if button released
        {
            P1IES |= BIT1; //sets interrupt to trigger on falling edge (button press)
            P4OUT &= ~BIT7; // toggle led
        }

    if(TA0CCR1 <= 1000) //checks if duty cycle is less than 100%
        {
            TA0CCR1 += 100; //adds 10% to duty cycle
        }
    else
        {
            TA0CCR1 = 0; // if duty cycle is at 100% it is reset to 0% not actually 0 b/c it does not trigger on 0
        }

    P1IFG &= ~BIT1; //Clears interupt flag register needed otherwise will be stuck in infinite loop
}
